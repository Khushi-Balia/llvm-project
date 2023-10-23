#ifndef PRUMCINSTLOWER_H
#define PRUMCINSTLOWER_H
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
class MCContext;
class MCInst;
class MCOperand;
class MachineInstr;
class MachineFunction;
class AsmPrinter;

/// This class is used to lower an MachineInstr into an MCInst.
class LLVM_LIBRARY_VISIBILITY PRUMCInstLower {
  MCContext &Ctx;
  AsmPrinter &Printer;

public:
  PRUMCInstLower(MCContext &Ctx, AsmPrinter &Printer)
  	: Ctx(Ctx), Printer(Printer) {}
  void Lower(const MachineInstr *MI, MCInst &OutMI) const;

  MCOperand LowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const;

  MCSymbol *GetGlobalAddressSymbol(const MachineOperand &MO) const;
};
}

#endif
