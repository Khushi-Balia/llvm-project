#include "PRUISelLowering.h"
#include "PRUTargetMachine.h"
#include "PRU.h"
#include "PRUSubtarget.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/ErrorHandling.h"

#include "PRUGenCallingConv.inc"

using namespace llvm;

const char *PRUTargetLowering::getTargetNodeName(unsigned Opcode) const {
#define NODE(name)                                                             \
  case PRUISD::name:                                                           \
    return #name

  switch (Opcode) {
  default:
    return nullptr;
    NODE(ADD);
    NODE(ADC);
    NODE(SUB);
    NODE(SUC);
    NODE(LSL);
    NODE(LSR);
    NODE(RSB);
    NODE(RSC);
    NODE(AND);
    NODE(OR );
    NODE(XOR);
    NODE(NOT);
    NODE(MIN);
    NODE(MAX);
    NODE(CLR);
    NODE(SET);
    NODE(LDI);
    NODE(LBBO);
    NODE(LBCO);
    NODE(SBBO);
    NODE(SBCO);
    NODE(LFC);
    NODE(STC);
    NODE(JAL);
    NODE(JMP);
    NODE(QBGT);
    NODE(QBLT);
    NODE(QBEQ);
    NODE(QBGE);
    NODE(QBLE);
    NODE(QBNE);
    NODE(QBA);
    NODE(QBBS);
    NODE(QBBC);
    NODE(LMBD);
    NODE(WBC);
    NODE(WBS);
    NODE(MOV);
    NODE(MVIB);
    NODE(MVIW);
    NODE(MVID);
    NODE(SCAN);
    NODE(HALT);    
    NODE(SLP);
    NODE(RET);
    NODE(ZERO);
    NODE(FILL);
    NODE(XIN);
    NODE(XOUT);
    NODE(XCHG);
    NODE(SXIN);
    NODE(SXOUT);
    NODE(SXCHG);
    NODE(LOOP);  
    NODE(ILOOP);
    NODE(MAXIDX);  
#undef NODE
  }
}

PRUTargetLowering::PRUTargetLowering(const PRUTargetMachine &TM,
                                               const PRUSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI) {
  // TODO
}

SDValue PRUTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    SmallVectorImpl<ISD::InputArg> const &Ins, SDLoc const &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  return Chain;
}

SDValue
PRUTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                    bool IsVarArg,
                                    SmallVectorImpl<ISD::OutputArg> const &Outs,
                                    SmallVectorImpl<SDValue> const &OutVals,
                                    SDLoc const &dl, SelectionDAG &DAG) const {
  return DAG.getNode(PRUISD::RET, Chain);

}