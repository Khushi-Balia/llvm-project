//===----------------------------------------------------------------------===//
//
// This file provides PRU specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef PRUMCTARGETDESC_H
#define PRUMCTARGETDESC_H

#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"

namespace llvm {
class Target;
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCSubtargetInfo;

class StringRef;
class raw_ostream;
class raw_pwrite_stream;
class Triple;

Target &getThePRUTarget();

MCCodeEmitter *createPRUMCCodeEmitter(const MCInstrInfo &MCII,
                                      const MCRegisterInfo &MRI,
                                      MCContext &Ctx);

MCAsmBackend *createPRUAsmBackend(const Target &T, const MCRegisterInfo &MRI,
                                  const Triple &TT, StringRef CPU,
                                  const MCTargetOptions &Options);

MCObjectWriter *createPRUELFObjectWriter(raw_pwrite_stream &OS, uint8_t OSABI);

} // End llvm namespace

#define GET_REGINFO_ENUM
#include "PRUGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "PRUGenInstrInfo.inc"

#endif
