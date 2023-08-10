//===----------------------------------------------------------------------===//
//
// This file provides PRU specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef PRUMCTARGETDESC_MCTARGETDESC_H_INCLUDED
#define PRUMCTARGETDESC_MCTARGETDESC_H_INCLUDED

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {

class MCCodeEmitter;
class MCInstrInfo;
class MCContext;

class Target;

MCCodeEmitter *createPRUMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx);

Target &getThePRUTarget();

} // namespace llvm

#define GET_REGINFO_ENUM
#include "PRUGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "PRUGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "PRUGenSubtargetInfo.inc"

#endif