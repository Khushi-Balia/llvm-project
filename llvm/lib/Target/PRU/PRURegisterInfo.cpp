//===----------------------------------------------------------------------===//
//
// This file contains the PRU implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "PRURegisterInfo.h"
#include "PRU.h"
#include "PRUSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetFrameLowering.h"
#include "llvm/Target/TargetInstrInfo.h"

#define GET_REGINFO_TARGET_DESC
#include "PRUGenRegisterInfo.inc"

using namespace llvm;

PRURegisterInfo::PRURegisterInfo() : PRUGenRegisterInfo(PRU::r1) {}

const MCPhysReg *
PRURegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CC_Save_SaveList; // from tablegen
}

BitVector PRURegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(PRU::r0); 
  Reserved.set(PRU::r1); 
  Reserved.set(PRU::r2); 
  
  return Reserved;
}

const TargetRegisterClass *
PRURegisterInfo::getPointerRegClass(const MachineFunction &MF, unsigned Kind)
                                                                         const {
  return &PRU::GPRegsRegClass;
}

void PRURegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  const MachineFunction &MF = *MBB.getParent();
  const MachineFrameInfo MFI = MF.getFrameInfo();
  MachineOperand &FIOp = MI.getOperand(FIOperandNum);
  unsigned FI = FIOp.getIndex();
    
  // Determine if we can eliminate the index from this kind of instruction.
  unsigned ImmOpIdx = 0;
  switch (MI.getOpcode()) {
  default:
    llvm_unreachable("Instruction not supported");
  case PRU::LD:
  case PRU::ST:
  case PRU::ADDri:
    ImmOpIdx = FIOperandNum + 1;
    break;
  }

  MachineOperand &ImmOp = MI.getOperand(ImmOpIdx);
  int Offset = MFI.getObjectOffset(FI) + MFI.getStackSize() + ImmOp.getImm();
  if (Offset % 4) {
    llvm_unreachable("Offset must be aligned to 4 bytes because memory is "
                     "32-bit word addressable only");
  }
  FIOp.ChangeToRegister(PRU::r2, false);
  ImmOp.setImm(Offset);
}

unsigned PRURegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  return PRU::r2;
}
