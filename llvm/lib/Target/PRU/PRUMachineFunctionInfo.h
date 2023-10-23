//===----------------------------------------------------------------------===//
//
// This file declares PRU-specific per-machine-function information.
//
//===----------------------------------------------------------------------===//

#ifndef MACHINEFUNCTIONINFO_H
#define MACHINEFUNCTIONINFO_H

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

class PRUMachineFunctionInfo : public MachineFunctionInfo {
  virtual void anchor();

  unsigned CalleeSavedFrameSize;

  int ReturnAddrIndex;

  int VarArgsFrameIndex;

public:
  PRUMachineFunctionInfo() : CalleeSavedFrameSize(0) {}

  explicit PRUMachineFunctionInfo(MachineFunction &MF)
    : CalleeSavedFrameSize(0), ReturnAddrIndex(0) {}

  unsigned getCalleeSavedFrameSize() const { return CalleeSavedFrameSize; }
  void setCalleeSavedFrameSize(unsigned bytes) { CalleeSavedFrameSize = bytes; }

  int getRAIndex() const { return ReturnAddrIndex; }
  void setRAIndex(int Index) { ReturnAddrIndex = Index; }

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex;}
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }
};

} // End llvm namespace

#endif
