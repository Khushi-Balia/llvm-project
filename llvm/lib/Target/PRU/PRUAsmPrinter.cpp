#include "PRU.h"
#include "PRUInstrInfo.h"
#include "PRUTargetMachine.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/CodeGen/AsmPrinter.h"

#define DEBUG_TYPE_A "pru-asm-printer"

namespace llvm {

class PRUAsmPrinter : public AsmPrinter {
public:
  PRUAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
     : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "PRU Assembly Printer"; }

    };
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePRUAsmPrinter() {
  llvm::RegisterAsmPrinter<llvm::PRUAsmPrinter> X(llvm::getThePRUTarget());
}
