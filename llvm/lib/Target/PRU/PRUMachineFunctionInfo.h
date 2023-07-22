#ifndef LLVM_LIB_TARGET_PRU_PRUMACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_PRU_PRUMACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include <map>

namespace llvm {
  // This class is derived from MachienFunction private PRU target-specific
  // information for each MachineFunction.
  class PRUMachineFunctionInfo : public MachineFunctionInfo {
  public:
    PRUMachineFunctionInfo(MachineFunction &MF)
        : MF(MF), VarArgsFrameIndex(0), MaxCallFrameSize(0) { }

    ~PRUMachineFunctionInfo();

    int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
    void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

  private:
    virtual void anchor();

    MachineFunction &MF;

    // Frame index for start of varargs area
    int VarArgsFrameIndex;

    unsigned MaxCallFrameSize;
  };
} // End llvm namespace
#endif