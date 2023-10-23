//===----------------------------------------------------------------------===//
//
// This file contains the PRU implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef PRUREGISTERINFO_H
#define PRUREGISTERINFO_H

#include "llvm/Target/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "PRUGenRegisterInfo.inc"

namespace llvm {

class TargetInstrInfo;

struct PRURegisterInfo : public PRUGenRegisterInfo {

  PRURegisterInfo();

  const uint16_t *getCalleeSavedRegs(const MachineFunction *MF = 0) const
      override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;

  const TargetRegisterClass*
  getPointerRegClass(const MachineFunction &MF,
                     unsigned Kind = 0) const override;

  void eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  unsigned getFrameRegister(const MachineFunction &MF) const override;
};
}

#endif
