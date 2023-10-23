//===----------------------------------------------------------------------===//
//
// This file provides PRU specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "PRUMCTargetDesc.h"
#include "PRUMCAsmInfo.h"
#include "InstPrinter/PRUInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

#define GET_INSTRINFO_MC_DESC
#include "PRUGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "PRUGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PRUGenRegisterInfo.inc"

using namespace llvm;

static MCInstrInfo *createPRUMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitPRUMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createPRUMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  InitPRUMCRegisterInfo(X, PRU::PC);
  return X;
}

static MCAsmInfo *createPRUMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT) { 
  return new PRUMCAsmInfo(TT);
}

static MCInstPrinter *
createPRUMCInstPrinter(const Triple &TT, unsigned SyntaxVariant,
                       const MCAsmInfo &MAI, const MCInstrInfo &MII,
                       const MCRegisterInfo &MRI) {
  return new PRUInstPrinter(MAI, MII, MRI);
}

extern "C" void LLVMInitializePRUTargetMC() {
  RegisterMCAsmInfoFn X(getThePRUTarget(), createPRUMCAsmInfo);
  TargetRegistry::RegisterMCInstrInfo(getThePRUTarget(), createPRUMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(getThePRUTarget(), createPRUMCRegisterInfo);
  TargetRegistry::RegisterMCAsmBackend(getThePRUTarget(), createPRUAsmBackend);
  TargetRegistry::RegisterMCCodeEmitter(getThePRUTarget(), createPRUMCCodeEmitter);
  TargetRegistry::RegisterMCInstPrinter(getThePRUTarget(), createPRUMCInstPrinter);
}
