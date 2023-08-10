//===----------------------------------------------------------------------===//
//
// This file contains the PRU implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "PRUInstrInfo.h"
#include "PRU.h"
#include "PRURegisterInfo.h"
#include "PRUTargetMachine.h"
#include "PRUSubtarget.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"

#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "PRUGenInstrInfo.inc"

using namespace llvm;

void PRUInstrInfo::anchor() {}

PRUInstrInfo::PRUInstrInfo()
    : PRUGenInstrInfo(PRU::ADJCALLSTACKDOWN,
                           PRU::ADJCALLSTACKUP) {}

unsigned PRUInstrInfo::GetInstSizeInBytes(const MachineInstr &MI) const {
  switch (MI.getOpcode()) {
    default:
      return MI.getDesc().getSize();
  }
}