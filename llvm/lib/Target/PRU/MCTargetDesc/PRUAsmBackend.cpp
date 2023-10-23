#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "MCTargetDesc/PRUFixupKinds.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCDirectives.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
class PRUAsmBackend : public MCAsmBackend {
  uint8_t OSABI;

public:
  PRUAsmBackend(const Target &T, const StringRef TT, uint8_t OSABI)
     : MCAsmBackend(), OSABI(OSABI) {}
  
  ~PRUAsmBackend() {}

  MCObjectWriter *createObjectWriter(raw_pwrite_stream &OS) const override;

  bool writeNopData(uint64_t Count, MCObjectWriter *OW) const override;

  unsigned getNumFixupKinds() const override {
    return PRU::NumTargetFixupKinds;
  }

    const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {
      const static MCFixupKindInfo Infos[PRU::NumTargetFixupKinds] = {

        // Name            Offset (bits) Size (bits)     Flags
        { "fixup_PRU_NONE",   0,  32,   0 },
        { "fixup_PRU_32",     0,  32,   0 },
      };

    if (Kind < FirstTargetFixupKind) {
      return MCAsmBackend::getFixupKindInfo(Kind);
    }

    assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
           "Invalid kind!");
    return Infos[Kind - FirstTargetFixupKind];
  }
  
  void applyFixup(const MCFixup &Fixup, char *Data, unsigned DataSize,
                  uint64_t Value, bool IsPCRel) const override;

  void processFixupValue(const MCAssembler &Asm, const MCAsmLayout &Layout,
                         const MCFixup &Fixup, const MCFragment *DF,
                         const MCValue &Target, uint64_t &Value,
                         bool &IsResolved) override;

  bool mayNeedRelaxation(const MCInst &Inst) const override {
    return false;
  }
  
  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    return false;
  }

  void relaxInstruction(const MCInst &Inst, const MCSubtargetInfo &STI,
                        MCInst &Res) const override {
    llvm_unreachable("PRUAsmBackend::relaxInstruction() unimplemented");
  }
};

bool PRUAsmBackend::writeNopData(uint64_t Count, MCObjectWriter *OW) const {
  if (Count == 0) {
      return true;
    }
  return false;
}

MCObjectWriter *PRUAsmBackend::createObjectWriter(raw_pwrite_stream &OS) const {
  return createPRUELFObjectWriter(OS, OSABI);
}
} 

static unsigned adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
                                 MCContext *Ctx = nullptr) {
  unsigned Kind = Fixup.getKind();
  switch (Kind) {
    default:
      llvm_unreachable("Unknown fixup kind!");
      case PRU::fixup_PRU_32:
        if (Value > 0xFFFF) {
          llvm_unreachable("Cannot process value larger than 16 bits");
        }
        return Value;
        break;
  } 
  return Value;
}

void PRUAsmBackend::processFixupValue(const MCAssembler &Asm,
                                      const MCAsmLayout &Layout,
                                      const MCFixup &Fixup,
                                      const MCFragment *DF,
                                      const MCValue &Target, uint64_t &Value,
                                      bool &IsResolved) {
  IsResolved = true;

  (void)adjustFixupValue(Fixup, Value, &Asm.getContext());
}

void PRUAsmBackend::applyFixup(const MCFixup &Fixup, char *Data,
                                 unsigned DataSize, uint64_t Value,
                                 bool IsPCRel) const {
  if (IsPCRel) {
    llvm_unreachable("PC Rel not currently implemented");
  }

  if (!Value) {
  Value = adjustFixupValue(Fixup, Value);
    return; 
  }

  unsigned Offset = Fixup.getOffset();

  assert(Offset <= DataSize && "Invalid fixup offset!");

  assert(Value%4 == 0 && "The destination address is not aligned to a word");
  Data[Offset] = Value & 0xFF;
  Data[Offset+1] = uint8_t((Value >> 8) & 0xFF);
  return;
}

MCAsmBackend *llvm::createPRUAsmBackend(const Target &T,
                                          const MCRegisterInfo &MRI,
                                          const Triple &TT, StringRef CPU,
                                          const MCTargetOptions &Options) {
  const uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(TT.getOS());
  return new PRUAsmBackend(T, TT.getTriple(), OSABI);
}
