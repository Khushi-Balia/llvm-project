//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that PRU uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#include "PRUISelLowering.h"
#include "PRU.h"
#include "PRURegisterInfo.h"
#include "PRUSubtarget.h"
#include "PRUTargetMachine.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "PRU-lower"

const char *PRUTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch ((PRUISD::NodeType)Opcode) {
    case PRUISD::FIRST_NUMBER:            break;
    case PRUISD::CMP:                     return "PRUISD::CMP";
    case PRUISD::RET_FLAG:                return "PRUISD::RET_FLAG";
    case PRUISD::BR_CC:                   return "PRUISD::BR_CC";
    case PRUISD::SELECT_CC:               return "PRUISD::SELECT_CC";
    case PRUISD::Wrapper:                 return "PRUISD::Wrapper";
    case PRUISD::CALL:                    return "PRUISD::CALL";
  }
  return nullptr;
}

PRUTargetLowering::PRUTargetLowering(const PRUTargetMachine &TM,
                                     const PRUSubtarget &STI)
    : TargetLowering(TM) {

  addRegisterClass(MVT::i32, &PRU::GPRegsRegClass);

  computeRegisterProperties(STI.getRegisterInfo());

  setStackPointerRegisterToSaveRestore(PRU::SP);

  // TODO: add all necessary setOperationAction calls

  setBooleanContents(ZeroOrOneBooleanContent);

  setOperationAction(ISD::BR_JT,            MVT::Other,   Expand);
  setOperationAction(ISD::BR_CC,            MVT::i32,     Custom);
  setOperationAction(ISD::BRCOND,           MVT::Other,   Expand);
  setOperationAction(ISD::SELECT,           MVT::i32,     Expand);
  setOperationAction(ISD::SELECT_CC,        MVT::i32,     Custom);
  setOperationAction(ISD::GlobalAddress,    MVT::i32,     Custom);
  setOperationAction(ISD::ExternalSymbol,   MVT::i32,     Custom);

  for (MVT VT : MVT::integer_valuetypes()) {
    for (auto N : {ISD::EXTLOAD, ISD::SEXTLOAD, ISD::ZEXTLOAD}) {
      setLoadExtAction(N, VT, MVT::i1, Promote);
      setLoadExtAction(N, VT, MVT::i8, Custom);
      setLoadExtAction(N, VT, MVT::i16, Custom);
    }
  }

}

SDValue PRUTargetLowering::LowerOperation(SDValue Op, SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
    case ISD::BR_CC:            return LowerBR_CC(Op, DAG);
    case ISD::SELECT_CC:        return LowerSELECT_CC(Op, DAG);
    case ISD::GlobalAddress:    return LowerGlobalAddress(Op, DAG);
    case ISD::ExternalSymbol:   return LowerExternalSymbol(Op, DAG);
    default:
      llvm_unreachable("unimplemented operand");
  }
}

#include "PRUGenCallingConv.inc"

// Transform physical registers into virtual registers
SDValue PRUTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  switch (CallConv) {
  default:
    llvm_unreachable("Unsupported calling convention");
  case CallingConv::C:
    break;
  }

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();

  if (IsVarArg) {
    llvm_unreachable("VarArg not supported");
  }

  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_PRU);

  for (auto &VA : ArgLocs) {
    if (VA.isRegLoc()) {

      EVT RegVT = VA.getLocVT();
      switch (RegVT.getSimpleVT().SimpleTy) {
      case MVT::i32: {
        const unsigned VReg =
            RegInfo.createVirtualRegister(&PRU::GPRegsRegClass);
        RegInfo.addLiveIn(VA.getLocReg(), VReg);
        SDValue ArgIn = DAG.getCopyFromReg(Chain, DL, VReg, RegVT);

        InVals.push_back(ArgIn);
        break;
      }
      default:
        DEBUG(dbgs() << "LowerFormalArguments Unhandled argument type: "
                     << RegVT.getEVTString() << "\n");
        llvm_unreachable("unhandled argument type");
      }
    } else {
      llvm_unreachable("Defined with too many args");
    }
  }
  return Chain;
}

SDValue
PRUTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::OutputArg> &Outs,
                               const SmallVectorImpl<SDValue> &OutVals,
                               const SDLoc &DL, SelectionDAG &DAG) const {
  if (IsVarArg) {
    report_fatal_error("VarArg not supported");
  }


  SmallVector<CCValAssign, 16> RVLocs;

  // Info about the registers and stack slot
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeReturn(Outs, RetCC_PRU);

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0, e = RVLocs.size(); i < e; ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Flag);

    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  RetOps[0] = Chain; 
  if (Flag.getNode()) {
    RetOps.push_back(Flag);
  }

  return DAG.getNode(PRUISD::RET_FLAG, DL, MVT::Other, RetOps);
}

static SDValue EmitCMP(SDValue &LHS, SDValue &RHS, SDValue &TargetCC,
                       ISD::CondCode CC, const SDLoc &dl, SelectionDAG &DAG) {

  assert(!LHS.getValueType().isFloatingPoint() && "We don't handle FP yet");

  PRUCC::CondCodes TCC = PRUCC::COND_INVALID;
  switch (CC) {
  default: llvm_unreachable("Invalid integer condition!");
  case ISD::SETULE:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  case ISD::SETUGT:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  case ISD::SETLE:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  case ISD::SETGT:
    std::swap(LHS, RHS);
    LLVM_FALLTHROUGH;
  }

  TargetCC = DAG.getConstant(TCC, dl, MVT::i32);
  return DAG.getNode(PRUISD::CMP, dl, MVT::Glue, LHS, RHS);
}

SDValue PRUTargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG) const {
  SDValue Chain = Op.getOperand(0);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(1))->get();
  SDValue LHS   = Op.getOperand(2);
  SDValue RHS   = Op.getOperand(3);
  SDValue Dest  = Op.getOperand(4);
  SDLoc dl  (Op);

  SDValue TargetCC;
  SDValue Flag = EmitCMP(LHS, RHS, TargetCC, CC, dl, DAG);

  return DAG.getNode(PRUISD::BR_CC, dl, Op.getValueType(),
                     Chain, Dest, TargetCC, Flag);
}

SDValue PRUTargetLowering::LowerSELECT_CC(SDValue Op,
                                             SelectionDAG &DAG) const {
  SDValue LHS    = Op.getOperand(0);
  SDValue RHS    = Op.getOperand(1);
  SDValue TrueV  = Op.getOperand(2);
  SDValue FalseV = Op.getOperand(3);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(4))->get();
  SDLoc dl   (Op);

  SDValue TargetCC;
  SDValue Flag = EmitCMP(LHS, RHS, TargetCC, CC, dl, DAG);

  SDVTList VTs = DAG.getVTList(Op.getValueType(), MVT::Glue);
  SDValue Ops[] = {TrueV, FalseV, TargetCC, Flag};

  return DAG.getNode(PRUISD::SELECT_CC, dl, VTs, Ops);
}

SDValue PRUTargetLowering::LowerGlobalAddress(SDValue Op,
                                              SelectionDAG &DAG) const {
  const GlobalValue *GV = cast<GlobalAddressSDNode>(Op)->getGlobal();
  int64_t Offset = cast<GlobalAddressSDNode>(Op)->getOffset();
  auto PtrVT = getPointerTy(DAG.getDataLayout());

  // Create the TargetGlobalAddress node, folding in the constant offset.
  SDValue Result = DAG.getTargetGlobalAddress(GV, SDLoc(Op), PtrVT, Offset);
  return DAG.getNode(PRUISD::Wrapper, SDLoc(Op), PtrVT, Result);
}

SDValue PRUTargetLowering::LowerExternalSymbol(SDValue Op,
                                               SelectionDAG &DAG) const {
  SDLoc dl(Op);
  const char *Sym = cast<ExternalSymbolSDNode>(Op)->getSymbol();
  auto PtrVT = getPointerTy(DAG.getDataLayout());
  SDValue Result = DAG.getTargetExternalSymbol(Sym, PtrVT);

  return DAG.getNode(PRUISD::Wrapper, dl, PtrVT, Result);
}

MachineBasicBlock *
PRUTargetLowering::EmitInstrWithCustomInserter(MachineInstr &MI,
                                               MachineBasicBlock *BB) const {
  unsigned Opc = MI.getOpcode();

  const TargetInstrInfo &TII = *BB->getParent()->getSubtarget().getInstrInfo();
  DebugLoc dl = MI.getDebugLoc();

  assert(Opc == PRU::Select && "Unexpected instr type to insert");

  const BasicBlock *LLVM_BB = BB->getBasicBlock();
  MachineFunction::iterator I = ++BB->getIterator();

  MachineBasicBlock *thisMBB = BB;
  MachineFunction *F = BB->getParent();
  MachineBasicBlock *copy0MBB = F->CreateMachineBasicBlock(LLVM_BB);
  MachineBasicBlock *copy1MBB = F->CreateMachineBasicBlock(LLVM_BB);
  F->insert(I, copy0MBB);
  F->insert(I, copy1MBB);
 
  copy1MBB->splice(copy1MBB->begin(), BB,
                   std::next(MachineBasicBlock::iterator(MI)), BB->end());
  copy1MBB->transferSuccessorsAndUpdatePHIs(BB);
  
  BB->addSuccessor(copy0MBB);
  BB->addSuccessor(copy1MBB);

  BuildMI(BB, dl, TII.get(PRU::JCC))
      .addMBB(copy1MBB)
      .addImm(MI.getOperand(3).getImm());

  BB = copy0MBB;

  // Update machine-CFG edges
  BB->addSuccessor(copy1MBB);

  BB = copy1MBB;
  BuildMI(*BB, BB->begin(), dl, TII.get(PRU::PHI), MI.getOperand(0).getReg())
      .addReg(MI.getOperand(2).getReg())
      .addMBB(copy0MBB)
      .addReg(MI.getOperand(1).getReg())
      .addMBB(thisMBB);

  MI.eraseFromParent(); 
  return BB;
}

template<typename ArgT>
static void ParseFunctionArgs(const SmallVectorImpl<ArgT> &Args,
                              SmallVectorImpl<unsigned> &Out) {
  unsigned CurrentArgIndex = ~0U;
  for (unsigned i = 0, e = Args.size(); i != e; i++) {
    if (CurrentArgIndex == Args[i].OrigArgIndex) {
      Out.back()++;
    } else {
      Out.push_back(1);
      CurrentArgIndex++;
    }
  }
}

template<typename ArgT>
static void AnalyzeArguments(CCState &State,
                             SmallVectorImpl<CCValAssign> &ArgLocs,
                             const SmallVectorImpl<ArgT> &Args) {
  static const MCPhysReg RegList[] = {
    PRU::R24, PRU::R25, PRU::R26, PRU::R27, PRU::R28, PRU::R29, PRU::R30,
    PRU::R31
  };
  static const unsigned NbRegs = array_lengthof(RegList);

  if (State.isVarArg()) {
    llvm_unreachable("Cannot support var args");
  }

  SmallVector<unsigned, 4> ArgsParts;
  ParseFunctionArgs(Args, ArgsParts);

  unsigned RegsLeft = NbRegs;
  bool UseStack = false;
  unsigned ValNo = 0;

  for (unsigned i = 0, e = ArgsParts.size(); i != e; i++) {
    MVT ArgVT = Args[ValNo].VT;
    ISD::ArgFlagsTy ArgFlags = Args[ValNo].Flags;
    MVT LocVT = ArgVT;
    CCValAssign::LocInfo LocInfo = CCValAssign::Full;

    if (ArgFlags.isByVal()) {
      State.HandleByVal(ValNo++, ArgVT, LocVT, LocInfo, 4, 4, ArgFlags);
      continue;
    }

    unsigned Parts = ArgsParts[i];

    if (!UseStack && Parts <= RegsLeft) {
      unsigned FirstVal = ValNo;
      for (unsigned j = 0; j < Parts; j++) {
        unsigned Reg = State.AllocateReg(RegList);
        State.addLoc(CCValAssign::getReg(ValNo++, ArgVT, Reg, LocVT, LocInfo));
        RegsLeft--;
      }

      SmallVectorImpl<CCValAssign>::iterator B = ArgLocs.begin() + FirstVal;
      std::reverse(B, B + Parts);
    } else {
      UseStack = true;
      for (unsigned j = 0; j < Parts; j++)
        CC_PRU(ValNo++, ArgVT, LocVT, LocInfo, ArgFlags, State);
    }
  }
}

SDValue PRUTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                     SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG                     = CLI.DAG;
  SDLoc &dl                             = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals     = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins   = CLI.Ins;
  SDValue Chain                         = CLI.Chain;
  SDValue Callee                        = CLI.Callee;
  bool &isTailCall                      = CLI.IsTailCall;
  CallingConv::ID CallConv              = CLI.CallConv;
  bool isVarArg                         = CLI.IsVarArg;

  // PRU target does not yet support tail call optimization.
  isTailCall = false;

  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), ArgLocs,
                 *DAG.getContext());
  AnalyzeArguments(CCInfo, ArgLocs, Outs);


  unsigned NumBytes = CCInfo.getNextStackOffset();
  auto PtrVT = getPointerTy(DAG.getDataLayout());

  Chain = DAG.getCALLSEQ_START(Chain,
                               DAG.getConstant(NumBytes, dl, PtrVT, true), dl);

  SmallVector<std::pair<unsigned, SDValue>, 4> RegsToPass;
  SmallVector<SDValue, 12> MemOpChains;
  SDValue StackPtr;

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];

    SDValue Arg = OutVals[i];

    // Promote the value if needed.
    switch (VA.getLocInfo()) {
      default: llvm_unreachable("Unknown loc info!");
      case CCValAssign::Full: break;
    }

    // Arguments that can be passed on register must be kept at RegsToPass
    // vector
    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    } else {
      assert(VA.isMemLoc());

      if (!StackPtr.getNode())
        StackPtr = DAG.getCopyFromReg(Chain, dl, PRU::r2, PtrVT);

      SDValue PtrOff =
          DAG.getNode(ISD::ADD, dl, PtrVT, StackPtr,
                      DAG.getIntPtrConstant(VA.getLocMemOffset(), dl));

      SDValue MemOp;
      ISD::ArgFlagsTy Flags = Outs[i].Flags;

      if (Flags.isByVal()) {
        SDValue SizeNode = DAG.getConstant(Flags.getByValSize(), dl, MVT::i32);
        MemOp = DAG.getMemcpy(Chain, dl, PtrOff, Arg, SizeNode,
                              Flags.getByValAlign(),
                              /*isVolatile*/false,
                              /*AlwaysInline=*/true,
                              /*isTailCall=*/false,
                              MachinePointerInfo(),
                              MachinePointerInfo());
      } else {
        MemOp = DAG.getStore(Chain, dl, Arg, PtrOff, MachinePointerInfo());
      }

      MemOpChains.push_back(MemOp);
    }
  }

  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, dl, MVT::Other, MemOpChains);

  SDValue InFlag;
  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i) {
    Chain = DAG.getCopyToReg(Chain, dl, RegsToPass[i].first,
                             RegsToPass[i].second, InFlag);
    InFlag = Chain.getValue(1);
  }

  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee))
    Callee = DAG.getTargetGlobalAddress(G->getGlobal(), dl, MVT::i32);
  else if (ExternalSymbolSDNode *E = dyn_cast<ExternalSymbolSDNode>(Callee))
    Callee = DAG.getTargetExternalSymbol(E->getSymbol(), MVT::i32);

  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  for (unsigned i = 0, e = RegsToPass.size(); i != e; ++i)
    Ops.push_back(DAG.getRegister(RegsToPass[i].first,
                                  RegsToPass[i].second.getValueType()));

  if (InFlag.getNode())
    Ops.push_back(InFlag);

  Chain = DAG.getNode(PRUISD::CALL, dl, NodeTys, Ops);
  InFlag = Chain.getValue(1);

  Chain = DAG.getCALLSEQ_END(Chain, DAG.getConstant(NumBytes, dl, PtrVT, true),
                             DAG.getConstant(0, dl, PtrVT, true), InFlag, dl);
  InFlag = Chain.getValue(1);

  return LowerCallResult(Chain, InFlag, CallConv, isVarArg, Ins, dl,
                         DAG, InVals);
}

static void AnalyzeRetResult(CCState &State,
                             const SmallVectorImpl<ISD::InputArg> &Ins) {
  State.AnalyzeCallResult(Ins, RetCC_PRU);
}

static void AnalyzeRetResult(CCState &State,
                             const SmallVectorImpl<ISD::OutputArg> &Outs) {
  State.AnalyzeReturn(Outs, RetCC_PRU);
}

template<typename ArgT>
static void AnalyzeReturnValues(CCState &State,
                                SmallVectorImpl<CCValAssign> &RVLocs,
                                const SmallVectorImpl<ArgT> &Args) {
  AnalyzeRetResult(State, Args);

  std::reverse(RVLocs.begin(), RVLocs.end());
}

/// LowerCallResult - Lower the result values of a call into the
/// appropriate copies out of appropriate physical registers.
SDValue PRUTargetLowering::LowerCallResult(
    SDValue Chain, SDValue InFlag, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, isVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  AnalyzeReturnValues(CCInfo, RVLocs, Ins);

  // Copy all of the result registers out of their specified physreg.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    Chain = DAG.getCopyFromReg(Chain, dl, RVLocs[i].getLocReg(),
                               RVLocs[i].getValVT(), InFlag).getValue(1);
    InFlag = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  }

  return Chain;
}
