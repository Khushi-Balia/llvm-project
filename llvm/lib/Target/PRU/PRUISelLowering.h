//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that PRU uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef PRUISELLOWERING_H
#define PRUISELLOWERING_H

#include "PRU.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"

namespace llvm {

class PRUSubtarget;

namespace PRUISD {
enum NodeType {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  RET_FLAG,
  CMP,
  BR_CC,
  SELECT_CC,
  Wrapper,
  CALL,
};
}

//===----------------------------------------------------------------------===//
// TargetLowering Implementation
//===----------------------------------------------------------------------===//

class PRUTargetLowering : public TargetLowering {
public:
  explicit PRUTargetLowering(const PRUTargetMachine &TM,
                             const PRUSubtarget &STI);

  virtual SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

  SDValue LowerBR_CC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerSELECT_CC(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG) const;
  SDValue LowerExternalSymbol(SDValue Op, SelectionDAG &DAG) const;

  // This method returns the name of a target specific DAG node.
  virtual const char *getTargetNodeName(unsigned Opcode) const override;

  MachineBasicBlock *
  EmitInstrWithCustomInserter(MachineInstr &MI,
                              MachineBasicBlock *BB) const override;

private:
  // Lower incoming arguments, copy physregs into vregs
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

  SDValue LowerCallResult(SDValue Chain, SDValue InFlag,
                          CallingConv::ID CallConv, bool isVarArg,
                          const SmallVectorImpl<ISD::InputArg> &Ins,
                          const SDLoc &dl, SelectionDAG &DAG,
                          SmallVectorImpl<SDValue> &InVals) const;
  
  bool shouldConvertConstantLoadToIntImm(const APInt &Imm,
                                         Type *Ty) const override {
    return true;
  }
};
}

#endif
