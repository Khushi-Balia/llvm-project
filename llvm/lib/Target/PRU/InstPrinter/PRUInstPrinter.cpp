//===----------------------------------------------------------------------===//
//
// This class prints an PRU MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "PRU.h"
#include "PRUInstPrinter.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#include "PRUGenAsmWriter.inc"

void PRUInstPrinter::printInst(const MCInst *MI, raw_ostream &O,
                                 StringRef Annot, const MCSubtargetInfo &STI) {
  printInstruction(MI, O);
  printAnnotation(O, Annot);
}

void PRUInstPrinter::printRegName(raw_ostream &O, unsigned RegNo) const {
  O << StringRef(getRegisterName(RegNo));
}

void PRUInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                  raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);

  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  if (Op.isExpr()) {
    Op.getExpr()->print(O, &MAI);
    return;
  }

  llvm_unreachable("Unknown operand");
}

void PRUInstPrinter::printMemSrcOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &O) {
  const MCOperand &BaseAddr = MI->getOperand(OpNo);
  const MCOperand &Offset = MI->getOperand(OpNo + 1);
  
  assert(Offset.isImm() && "Expected immediate in displacement field");

  O << "M[";
  printRegName(O, BaseAddr.getReg());
  unsigned OffsetVal = Offset.getImm();
  if (OffsetVal) {
    O << "+" << Offset.getImm();
  }
  O << "]";
}

void PRUInstPrinter::printPCRelImmOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm())
    O << Op.getImm();
  else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void PRUInstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  default:
   llvm_unreachable("Unsupported CC code");
  }
}
