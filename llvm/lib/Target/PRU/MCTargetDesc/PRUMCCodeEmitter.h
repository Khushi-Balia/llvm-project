#ifndef PRUCODEEMITTER_H_INCLUDED
#define PRUCODEEMITTER_H_INCLUDED

#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/Support/DataTypes.h"

#define GET_INSTRINFO_OPERAND_TYPES_ENUM
#include "PRUGenInstrInfo.inc"

namespace llvm {
class MCInstrInfo;
class MCContext;
class MCFixup;
class raw_ostream;

class PRUMCCodeEmitter : public MCCodeEmitter {
  MCInstrInfo const &MCII;
  MCContext &Ctx;
  bool IsLittleEndian;

public:
  PRUMCCodeEmitter(MCInstrInfo const &MCII, MCContext &Ctx, bool IsLittle=false);

  void encodeInstruction(MCInst const &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         MCSubtargetInfo const &STI) const override;
};

} // namespace llvm

#endif