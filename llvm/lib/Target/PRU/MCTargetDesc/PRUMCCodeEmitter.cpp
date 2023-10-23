//===----------------------------------------------------------------------===//
//
// This file implements the PRUMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/PRUFixupKinds.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace {
class PRUMCCodeEmitter : public MCCodeEmitter {
  PRUMCCodeEmitter(const PRUMCCodeEmitter &) = delete;
  void operator=(const PRUMCCodeEmitter &) = delete;
  const MCInstrInfo &MCII;
  const MCContext &CTX;

public:
  PRUMCCodeEmitter(const MCInstrInfo &MCII, MCContext &CTX)
      : MCII(MCII), CTX(CTX) {}

  ~PRUMCCodeEmitter() override {}

  void encodeInstruction(const MCInst &MI, raw_ostream &OS,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  /// TableGen generated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getMemSrcValue(const MCInst &MI, unsigned OpIdx,
                          SmallVectorImpl<MCFixup> &Fixups,
                          const MCSubtargetInfo &STI) const;

  void emitByte(unsigned char C, raw_ostream &OS) const { OS << (char)C; }

  void emitConstant(uint64_t Val, unsigned Size, raw_ostream &OS) const {
 
    for (unsigned i = 0; i != Size; ++i) {
      emitByte(Val & 255, OS);
      Val >>= 8;
    }
  }

};
} // end anonymous namespace

MCCodeEmitter *llvm::createPRUMCCodeEmitter(const MCInstrInfo &MCII,
                                            const MCRegisterInfo &MRI,
                                            MCContext &CTX) {
  return new PRUMCCodeEmitter(MCII, CTX);
}

unsigned
PRUMCCodeEmitter::getMachineOpValue(const MCInst &MI,
                                    const MCOperand &MO,
                                    SmallVectorImpl<MCFixup> &Fixups,
                                    const MCSubtargetInfo &STI) const {

  if (MO.isReg()) {
    return CTX.getRegisterInfo()->getEncodingValue(MO.getReg());
  }

  if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  }
    
  if (MO.isExpr()) {
    const MCExpr *Expr = MO.getExpr();
    MCExpr::ExprKind Kind = Expr->getKind();

    if (Kind == MCExpr::Constant) {
      return cast<MCConstantExpr>(Expr)->getValue();
    }

    if (Kind == MCExpr::Binary) {
      Expr = static_cast<const MCBinaryExpr*>(Expr)->getLHS();
      Kind = Expr->getKind();
    }

    assert (Kind == MCExpr::SymbolRef && "unknown MCExpr kind");

    PRU::Fixups FixupKind = PRU::Fixups(0);
      
    switch(cast<MCSymbolRefExpr>(Expr)->getKind()) {
    default:
      llvm_unreachable("Unknown fixup kind!");
    case MCSymbolRefExpr::VK_None:
      FixupKind = PRU::fixup_PRU_32;
      break;
    } // switch

    Fixups.push_back(MCFixup::create(0, Expr, MCFixupKind(FixupKind)));
    return 0;
  }

  llvm_unreachable("Unhandled expression!");
  return 0;
}

unsigned PRUMCCodeEmitter::getMemSrcValue(const MCInst &MI, unsigned OpIdx,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  unsigned Bits = 0;
  const MCOperand &RegOp = MI.getOperand(OpIdx);
  const MCOperand &ImmOp = MI.getOperand(OpIdx + 1);
  Bits |= (getMachineOpValue(MI, RegOp, Fixups, STI) << 16);
  Bits |= (unsigned)ImmOp.getImm() & 0xffff;
  return Bits;
}

void PRUMCCodeEmitter::encodeInstruction(const MCInst &MI, raw_ostream &OS,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  if (Desc.getSize() != 4) {
    llvm_unreachable("Unexpected instruction size!");
  }

  const uint32_t Binary = getBinaryCodeForInstr(MI, Fixups, STI);

  emitConstant(Binary, Desc.getSize(), OS);
  ++MCNumEmitted;
}

#include "PRUGenMCCodeEmitter.inc"
