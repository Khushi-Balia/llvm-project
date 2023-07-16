#include "PRUSubtarget.h"
#include "PRU.h"
#include "PRUISelLowering.h"
#include "PRURegisterInfo.h"
#include "PRUTargetMachine.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/TargetRegistry.h"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR

#include "PRUGenSubtargetInfo.inc"

#define DEBUG_TYPE "pru-subtarget"

namespace llvm {

PRUSubtarget::PRUSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, const PRUTargetMachine &TM)
    : PRUGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      TLInfo(TM, initializeSubtargetDependencies(CPU, FS, TM)) {
  // Parse features string.
  ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);
}

PRUSubtarget &
PRUSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                              const TargetMachine &TM) {
  // Parse features string.
  ParseSubtargetFeatures(CPU, /*TuneCPU*/ CPU, FS);
  return *this;
}
}