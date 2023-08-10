//===----------------------------------------------------------------------===//
//
// This file provides PRU specific target descirptions.
//
//===----------------------------------------------------------------------===//

#include "PRUMCTargetDesc.h"
#include "PRUMCAsmInfo.h"
#include "PRUInstPrinter.h"
#include "PRUTargetStreamer.h"
#include "PRUMCCodeEmitter.h"
#include "TargetInfo/PRUTargetInfo.h"

#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"


#define GET_INSTRINFO_MC_DESC
#include "PRUGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "PRUGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "PRUGenRegisterInfo.inc"

namespace llvm
{

MCCodeEmitter *createPRUMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx) {
  return new PRUMCCodeEmitter(MCII, Ctx);
}

static MCInstrInfo *createPRUMCInstrInfo() {
  MCInstrInfo *I = new MCInstrInfo();
  // This function is generated in PRUGenInstrInfo.inc
  InitPRUMCInstrInfo(I);
  return I;
}

static MCRegisterInfo *createPRUMCRegisterInfo(Triple const &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  // This function is generated in PRUGenRegisterInfo.inc
  InitPRUMCRegisterInfo(X, 0);
  return X;
}

static MCSubtargetInfo *
createPRUMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  // This function is generated in PRUGenSubtargetInfo.inc
  return createPRUMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCInstPrinter *createPRUMCInstPrinter(Triple const &T,
                                                  unsigned SyntaxVariant,
                                                  MCAsmInfo const &MAI,
                                                  const MCInstrInfo &MII,
                                                  MCRegisterInfo const &MRI) {
  return new PRUInstPrinter(MAI, MII, MRI);
}

static MCTargetStreamer *
createPRUAsmTargetStreamer(MCStreamer &S, formatted_raw_ostream &OS,
                                MCInstPrinter *_InstPrint, bool _isVerboseAsm) {
  return new PRUTargetAsmStreamer(S, OS);
}

static void initializeTarget(Target &T) {
  RegisterMCAsmInfo<PRUMCAsmInfo> X(T);
  TargetRegistry::RegisterMCInstrInfo(T, createPRUMCInstrInfo);
  TargetRegistry::RegisterMCRegInfo(T, createPRUMCRegisterInfo);
  TargetRegistry::RegisterMCSubtargetInfo(T, createPRUMCSubtargetInfo);
  TargetRegistry::RegisterMCInstPrinter(T, createPRUMCInstPrinter);
  TargetRegistry::RegisterAsmTargetStreamer(T, createPRUAsmTargetStreamer);
  // TODO: TargetRegistry::RegisterObjectTargetStreamer
  TargetRegistry::RegisterMCCodeEmitter(T, createPRUMCCodeEmitter);
}

// This function will be called by llc via C preprocessor
extern "C" void LLVMInitializePRUTargetMC() {
  initializeTarget(getThePRUTarget());

}
  
} // namespace llvm