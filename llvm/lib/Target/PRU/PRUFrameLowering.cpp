//===----------------------------------------------------------------------===//
//
// This file contains the PRU implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "PRUFrameLowering.h"
#include "PRUInstrInfo.h"
#include "PRUMachineFunctionInfo.h"
#include "PRUSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

PRUFrameLowering::PRUFrameLowering(const PRUSubtarget &STI)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsUp, 
                          /*StackAlignment=*/4,
                          /*LocalAreaOffset=*/0) {}

bool PRUFrameLowering::hasFP(const MachineFunction &MF) const { return true; }

void PRUFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

void PRUFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {}

MachineBasicBlock::iterator
PRUFrameLowering::eliminateCallFramePseudoInstr(MachineFunction &MF,
												MachineBasicBlock &MBB,
    											MachineBasicBlock::iterator I)
																		 const {
  const PRUInstrInfo &TII =
      *static_cast<const PRUInstrInfo *>(MF.getSubtarget().getInstrInfo());
  unsigned StackAlign = getStackAlignment();

  if (!hasReservedCallFrame(MF)) {
    MachineInstr &Old = *I;
    uint64_t Amount = Old.getOperand(0).getImm();
    if (Amount != 0) {
      // keep the stack aligned properly. 
      Amount = (Amount+StackAlign-1)/StackAlign*StackAlign;

      MachineInstr *New = nullptr;
      if (Old.getOpcode() == TII.getCallFrameSetupOpcode()) {
        New =
            BuildMI(MF, Old.getDebugLoc(), TII.get(PRU::SUBri), PRU::r2)
                .addReg(PRU::r2)
                .addImm(Amount);
      } else {
        assert(Old.getOpcode() == TII.getCallFrameDestroyOpcode());
        uint64_t CalleeAmt = Old.getOperand(1).getImm();
        Amount -= CalleeAmt;
        if (Amount)
          New = BuildMI(MF, Old.getDebugLoc(), TII.get(PRU::ADDri),
                        PRU::r2)
                    .addReg(PRU::r2)
                    .addImm(Amount);
      }

      if (New) {
        New->getOperand(3).setIsDead();

        // Replace the pseudo instruction with a new instruction
        MBB.insert(I, New);
      }
    }
  } else if (I->getOpcode() == TII.getCallFrameDestroyOpcode()) {
    if (uint64_t CalleeAmt = I->getOperand(1).getImm()) {
      MachineInstr &Old = *I;
      MachineInstr *New =
          BuildMI(MF, Old.getDebugLoc(), TII.get(PRU::SUBri), PRU::r2)
              .addReg(PRU::r2)
              .addImm(CalleeAmt);
      New->getOperand(3).setIsDead();

      MBB.insert(I, New);
    }
  }

  return MBB.erase(I);
}

bool
PRUFrameLowering::spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                            MachineBasicBlock::iterator MI,
                                        const std::vector<CalleeSavedInfo> &CSI,
                                        const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  PRUMachineFunctionInfo *MFI = MF.getInfo<PRUMachineFunctionInfo>();
  MFI->setCalleeSavedFrameSize(CSI.size() * 2);

  for (unsigned i = CSI.size(); i != 0; --i) {
    unsigned Reg = CSI[i-1].getReg();
    MBB.addLiveIn(Reg);
    BuildMI(MBB, MI, DL, TII.get(PRU::PUSH))
      .addReg(Reg, RegState::Kill);
  }
  return true;
}

bool
PRUFrameLowering::restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                              MachineBasicBlock::iterator MI,
                                        const std::vector<CalleeSavedInfo> &CSI,
                                        const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  for (unsigned i = 0, e = CSI.size(); i != e; ++i)
    BuildMI(MBB, MI, DL, TII.get(PRU::POP), CSI[i].getReg());

  return true;
}

bool PRUFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  return !MF.getFrameInfo().hasVarSizedObjects();
}

void PRUFrameLowering::processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                                         RegScavenger *) const {
  // Create a frame entry for the FP register that must be saved.
  if (hasFP(MF)) {
    int FrameIdx = MF.getFrameInfo().CreateFixedObject(4, -4, true);
    (void)FrameIdx;
    assert(FrameIdx == MF.getFrameInfo().getObjectIndexBegin() &&
           "Slot for FP register must be last in order to be found!");
  }
}
