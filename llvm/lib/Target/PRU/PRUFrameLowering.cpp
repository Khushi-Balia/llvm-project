#include "PRUFrameLowering.h"
#include "PRUInstrInfo.h"
#include "PRUSubtarget.h"
#include "PRUTargetMachine.h"
#include "PRU.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"


using namespace llvm;

PRUFrameLowering::PRUFrameLowering()
    : TargetFrameLowering(StackGrowsDown, Align(16), 0) {}

void PRUFrameLowering::emitPrologue(MachineFunction &MF,
                                         MachineBasicBlock &MBB) const {
   const PRUSubtarget &STI = MF.getSubtarget<PRUSubtarget>();
  // TODO
}

void PRUFrameLowering::emitEpilogue(MachineFunction &MF,
                                         MachineBasicBlock &MBB) const {
  // TODO
}

bool PRUFrameLowering::hasFP(const MachineFunction &MF) const {
  const PRUSubtarget &STI = MF.getSubtarget<PRUSubtarget>();
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();

  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken();

  return false;
}

MachineBasicBlock::iterator
PRUFrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  // TODO
   const PRUSubtarget &STI = MF.getSubtarget<PRUSubtarget>();
  return MBB.erase(I);
}