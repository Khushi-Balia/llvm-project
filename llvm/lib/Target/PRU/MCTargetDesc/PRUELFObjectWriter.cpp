#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
  class PRUELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    PRUELFObjectWriter(uint8_t OSABI);

    virtual ~PRUELFObjectWriter();

    unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                          const MCFixup &Fixup, bool IsPCRel) const override;
    };
}

PRUELFObjectWriter::PRUELFObjectWriter(uint8_t OSABI)
    : MCELFObjectTargetWriter(/*Is64Bit*/ false, OSABI,
                              /*ELF::EM_PRU*/ ELF::EM_PRU,
                              /*HasRelocationAddend*/ false) {}

PRUELFObjectWriter::~PRUELFObjectWriter() {}

unsigned PRUELFObjectWriter::getRelocType(MCContext &Ctx,
                                            const MCValue &Target,
                                            const MCFixup &Fixup,
                                            bool IsPCRel) const {
  llvm_unreachable("PRUELFObjectWriter: invalid fixup kind");
}

MCObjectWriter *llvm::createPRUELFObjectWriter(raw_pwrite_stream &OS,
                                                uint8_t OSABI) {
  MCELFObjectTargetWriter *MOTW = new PRUELFObjectWriter(OSABI);
  return createELFObjectWriter(MOTW, OS, /*IsLittleEndian=*/true);
}
