#ifndef PRUREGISTERINFO_H_INCLUDED
#define PRUREGISTERINFO_H_INCLUDED

#include "PRU.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "PRUGenRegisterInfo.inc"

namespace llvm {

class PRURegisterInfo : public PRUGenRegisterInfo {

public:
  PRURegisterInfo();

private:

  MCPhysReg const *getCalleeSavedRegs(MachineFunction const *MF) const override;
  uint32_t const *getCallPreservedMask(MachineFunction const &MF,
                                       CallingConv::ID CC) const override;
  BitVector getReservedRegs(const MachineFunction &MF) const override;
  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;
  Register getFrameRegister(MachineFunction const &MF) const override;
};

} // namespace llvm

#endif 
