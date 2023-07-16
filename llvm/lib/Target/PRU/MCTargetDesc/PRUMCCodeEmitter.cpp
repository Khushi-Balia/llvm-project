#include "MCTargetDesc/PRUMCCodeEmitter.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"

#define DEBUG_TYPE "PRU-code-emitter"

#define GET_INSTRMAP_INFO
#include "PRUGenInstrInfo.inc"
#undef GET_INSTRMAP_INFO

using namespace llvm;

PRUMCCodeEmitter::PRUMCCodeEmitter(MCInstrInfo const &MCII,
                                             MCContext &Ctx, bool IsLittle)
    : MCII(MCII), Ctx(Ctx), IsLittleEndian(IsLittle) {}

void PRUMCCodeEmitter::encodeInstruction(
    MCInst const &MI, raw_ostream &OS, SmallVectorImpl<MCFixup> &Fixups,
    MCSubtargetInfo const &STI) const {
  // TODO
}
