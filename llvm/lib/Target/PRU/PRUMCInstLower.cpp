//===----------------------------------------------------------------------===//
//
// This file contains code to lower PRU MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "PRU.h"
#include "PRUMCInstLower.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

void PRUMCInstLower::Lower(const MachineInstr *MI, MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);

    MCOperand MCOp;
    switch (MO.getType()) {
    default:
      MI->dump();
      llvm_unreachable("PRUMCInstLower::Lower() unknown operand type");
    case MachineOperand::MO_Register:
      // Ignore all implicit register operands.
      if (MO.isImplicit()) {
        continue;
      }
      MCOp = MCOperand::createReg(MO.getReg());
      break;
    case MachineOperand::MO_Immediate:
      MCOp = MCOperand::createImm(MO.getImm());
      break;
    case MachineOperand::MO_MachineBasicBlock:
      MCOp = MCOperand::createExpr(MCSymbolRefExpr::create(
                         MO.getMBB()->getSymbol(), Ctx));
      break;
    case MachineOperand::MO_GlobalAddress:
      MCOp = LowerSymbolOperand(MO, GetGlobalAddressSymbol(MO));
      break;
    case MachineOperand::MO_RegisterMask:
      continue;
    }
    OutMI.addOperand(MCOp);
  }
}

MCOperand PRUMCInstLower::LowerSymbolOperand(const MachineOperand &MO,
                                             MCSymbol *Sym) const {

  const MCExpr *Expr = MCSymbolRefExpr::create(Sym, Ctx);

  switch (MO.getTargetFlags()) {
    default: llvm_unreachable("Unknown target flag on GV operand");
    case 0: break;
  }

  if (!MO.isJTI() && MO.getOffset())
    Expr = MCBinaryExpr::createAdd(Expr,
                                   MCConstantExpr::create(MO.getOffset(), Ctx),
                                   Ctx);
  return MCOperand::createExpr(Expr);
}

MCSymbol
*PRUMCInstLower::GetGlobalAddressSymbol(const MachineOperand &MO) const {
  switch (MO.getTargetFlags()) {
    default: llvm_unreachable("Unknown target flag on GV operand");
    case 0: break;
  }

  return Printer.getSymbol(MO.getGlobal());
}
