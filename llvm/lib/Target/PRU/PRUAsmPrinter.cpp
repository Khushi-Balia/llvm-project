#include "PRU.h"
#include "PRUInstrInfo.h"
#include "PRUTargetMachine.h"

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/TargetRegisterInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSectionELF.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetLoweringObjectFile.h"


#define DEBUG_TYPE_A "pru-asm-printer"

namespace llvm {

class PRUAsmPrinter : public AsmPrinter {
public:
  PRUAsmPrinter(TargetMachine &TM, std::unique_ptr<MCStreamer> Streamer)
     : AsmPrinter(TM, std::move(Streamer)) {}

  StringRef getPassName() const override { return "PRU Assembly Printer"; }

   void emitInstruction(const MachineInstr *MI) override;

    };

  void PRUAsmPrinter::printOperand(const MachineInstr *MI, unsigned OpNo,
                                 raw_ostream &O) {
  const MachineOperand &MO = MI->getOperand(OpNo);

  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    O << PRUInstPrinter::getPrettyRegisterName(MO.getReg(), MRI);
    break;
  case MachineOperand::MO_Immediate:
    O << MO.getImm();
    break;
  case MachineOperand::MO_GlobalAddress:
    O << getSymbol(MO.getGlobal());
    break;
  case MachineOperand::MO_ExternalSymbol:
    O << *GetExternalSymbolSymbol(MO.getSymbolName());
    break;
  case MachineOperand::MO_MachineBasicBlock:
    O << *MO.getMBB()->getSymbol();
    break;
  default:
    llvm_unreachable("Not implemented yet!");
  }

  void PRUAsmPrinter::EmitInstruction(const MachineInstr *MI) {

  PRUMCInstLower MCInstLowering(OutContext, *this);

  MCInst I;
  MCInstLowering.lowerInstruction(*MI, I);
  EmitToStreamer(*OutStreamer, I);
  }
}

  MachineBasicBlock::const_instr_iterator I = MI->getIterator();
  MachineBasicBlock::const_instr_iterator E = MI->getParent()->instr_end();

} 

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePRUAsmPrinter() {
  llvm::RegisterAsmPrinter<llvm::PRUAsmPrinter> X(llvm::getThePRUTarget());
}
