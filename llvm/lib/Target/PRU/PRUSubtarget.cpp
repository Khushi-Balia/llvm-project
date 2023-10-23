//===----------------------------------------------------------------------===//
//
// This file implements the PRU specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "PRU.h"
#include "PRUFrameLowering.h"
#include "PRUSubtarget.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "PRU-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "PRUGenSubtargetInfo.inc"

void PRUSubtarget::anchor() {}

PRUSubtarget::PRUSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                           PRUTargetMachine &TM)
    : PRUGenSubtargetInfo(TT, CPU, FS), InstrInfo(), FrameLowering(*this),
      TLInfo(TM, *this) {}
