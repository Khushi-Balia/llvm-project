//===----------------------------------------------------------------------===//
//
// This file contains the PRU implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "PRUInstrInfo.h"
#include "PRU.h"
#include "PRUSubtarget.h"
#include "PRUTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "PRUGenInstrInfo.inc"

using namespace llvm;

void PRUInstrInfo::anchor() {}

PRUInstrInfo::PRUInstrInfo() : PRUGenInstrInfo(), RI() {}

void PRUInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator I,
                                  const DebugLoc &DL, unsigned DestReg,
                                  unsigned SrcReg, bool KillSrc) const {

  if (PRU::GPRegsRegClass.contains(DestReg, SrcReg)) {
      BuildMI(MBB, I, DL, get(PRU::CPYrr), DestReg)
        .addReg(SrcReg, getKillRegState(KillSrc));
  }
  else {
    llvm_unreachable("Impossible reg-to-reg copy");
  }
}

void PRUInstrInfo::storeRegToStackSlot(MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator MI,
                                       unsigned SrcReg, bool isKill,
                                       int FrameIdx,
                                       const TargetRegisterClass *RC,
                                       const TargetRegisterInfo *TRI) const {
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  BuildMI(MBB, MI, DL, get(PRU::PUSH))
    .addReg(SrcReg, getKillRegState(isKill));
}

void PRUInstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                        MachineBasicBlock::iterator MI,
                                        unsigned DestReg, int FrameIdx,
                                        const TargetRegisterClass *RC,
                                        const TargetRegisterInfo *TRI) const{
  DebugLoc DL;
  if (MI != MBB.end()) DL = MI->getDebugLoc();
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  BuildMI(MBB, MI, DL, get(PRU::POP))
    .addReg(DestReg, getDefRegState(true));
}

bool PRUInstrInfo::
reverseBranchCondition(SmallVectorImpl<MachineOperand> &Cond) const {
  assert(Cond.size() == 1 && "Invalid Xbranch condition!");

  PRUCC::CondCodes CC = static_cast<PRUCC::CondCodes>(Cond[0].getImm());

  Cond[0].setImm(CC);
  return false;
}

bool PRUInstrInfo::isUnpredicatedTerminator(const MachineInstr &MI) const {
  if (!MI.isTerminator())
    return false;

  if (MI.isBranch() && !MI.isBarrier())
    return true;
  if (!MI.isPredicable())
    return true;
  return !isPredicated(MI);
}

bool PRUInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                 MachineBasicBlock *&TBB,
                                 MachineBasicBlock *&FBB,
                                 SmallVectorImpl<MachineOperand> &Cond,
                                 bool AllowModify) const {
  // Start from the bottom of the block and work up, examining the
  // terminator instructions.
  MachineBasicBlock::iterator I = MBB.end();
  while (I != MBB.begin()) {
    --I;
    if (I->isDebugValue())
      continue;

    if (!isUnpredicatedTerminator(*I))
      break;

    if (!I->isBranch())
      return true;

    if (I->getOpcode() == PRU::JMP) {
      if (!AllowModify) {
        TBB = I->getOperand(0).getMBB();
        continue;
      }

      // If the block has any instructions after a JMP, delete them.
      while (std::next(I) != MBB.end())
        std::next(I)->eraseFromParent();
      Cond.clear();
      FBB = nullptr;

      if (MBB.isLayoutSuccessor(I->getOperand(0).getMBB())) {
        TBB = nullptr;
        I->eraseFromParent();
        I = MBB.end();
        continue;
      }

      // TBB is used to indicate the unconditinal destination.
      TBB = I->getOperand(0).getMBB();
      continue;
    }

      
    PRUCC::CondCodes BranchCode =
      static_cast<PRUCC::CondCodes>(I->getOperand(1).getImm());
    if (BranchCode == PRUCC::COND_INVALID)
      return true;  

    if (Cond.empty()) {
      FBB = TBB;
      TBB = I->getOperand(0).getMBB();
      Cond.push_back(MachineOperand::CreateImm(BranchCode));
      continue;
    }

    assert(Cond.size() == 1);
    assert(TBB);

    if (TBB != I->getOperand(0).getMBB())
      return true;

    PRUCC::CondCodes OldBranchCode = (PRUCC::CondCodes)Cond[0].getImm();
    if (OldBranchCode == BranchCode)
      continue;

    return true;
  }

  return false;
}

unsigned PRUInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                    int *BytesRemoved) const {
  assert(!BytesRemoved && "code size not handled");

  MachineBasicBlock::iterator I = MBB.end();
  unsigned Count = 0;

  while (I != MBB.begin()) {
    --I;
    if (I->isDebugValue())
      continue;
    if (I->getOpcode() != PRU::JMP &&
        I->getOpcode() != PRU::JAL
        )
      break;

    I->eraseFromParent();
    I = MBB.end();
    ++Count;
  }

  return Count;
}

unsigned PRUInstrInfo::insertBranch(MachineBasicBlock &MBB,
                                       MachineBasicBlock *TBB,
                                       MachineBasicBlock *FBB,
                                       ArrayRef<MachineOperand> Cond,
                                       const DebugLoc &DL,
                                       int *BytesAdded) const {
  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  assert((Cond.size() == 1 || Cond.size() == 0) &&
         "PRU branch conditions have one component!");
  assert(!BytesAdded && "code size not handled");

  if (Cond.empty()) {
    // Unconditional branch?
    assert(!FBB && "Unconditional branch with multiple successors!");
    BuildMI(&MBB, DL, get(PRU::JMP)).addMBB(TBB);
    return 1;
  }

  if (FBB) {
    BuildMI(&MBB, DL, get(PRU::JMP)).addMBB(FBB);
    ++Count;
  }
  return Count;
}
