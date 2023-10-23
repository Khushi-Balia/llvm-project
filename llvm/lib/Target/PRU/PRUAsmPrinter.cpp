//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to the PRU assembly language.
//
//===----------------------------------------------------------------------===//

#define DEBUG_TYPE "asm-printer"
#include "PRU.h"
#include "InstPrinter/PRUInstPrinter.h"
#include "PRUInstrInfo.h"
#include "PRUMCInstLower.h"
#include "PRUSubtarget.h"
#include "PRUTargetMachine.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineJumpTableInfo.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include <algorithm>
#include <cctype>

using namespace llvm;

namespace {
  class PRUAsmPrinter : public AsmPrinter {
    PRUMCInstLower MCInstLowering;

  public:
    explicit PRUAsmPrinter(TargetMachine &TM,
                           std::unique_ptr<MCStreamer> Streamer)
        : AsmPrinter(TM, std::move(Streamer)),
          MCInstLowering(OutContext, *this) {}

    virtual StringRef getPassName() const {
        return StringRef("PRU Assembly Printer");
    }
      
    void EmitFunctionEntryLabel();
    void EmitInstruction(const MachineInstr *MI);
    void EmitFunctionBodyStart();
  };
} // end of anonymous namespace

void PRUAsmPrinter::EmitFunctionBodyStart() {}

void PRUAsmPrinter::EmitFunctionEntryLabel() {
  OutStreamer->EmitLabel(CurrentFnSym);
}

void PRUAsmPrinter::EmitInstruction(const MachineInstr *MI) {
  MCInst TmpInst;
  MCInstLowering.Lower(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

extern "C" void LLVMInitializePRUAsmPrinter() {
  RegisterAsmPrinter<PRUAsmPrinter> X(getThePRUTarget());
}
