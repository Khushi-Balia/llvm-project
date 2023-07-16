include "llvm/Target/Target.td"

include "PRURegisterInfo.td"
include "PRUInstrInfo.td"
include "PRUCallingConv.td"
include "PRUDevices.td"

def PRUInstrInfo : InstrInfo;

//===---------------------------------------------------------------------===//
// Assembly Printers
//===---------------------------------------------------------------------===//

def PRUAsmWriter : AsmWriter {
  string AsmWriterClassName = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===---------------------------------------------------------------------===//
// Assembly Parsers
//===---------------------------------------------------------------------===//

def PRUAsmParser : AsmParser {
  let ShouldEmitMatchRegisterName = 1;
  let ShouldEmitMatchRegisterAltName = 1;
}

def PRUAsmParserVariant : AsmParserVariant {
  int Variant = 0;

  // Recognize hard coded registers.
  string RegisterPrefix = "$";
  string TokenizingCharacters = "+";
}

def PRU : Target {
  let InstructionSet = PRUInstrInfo;
  let AssemblyWriters = [PRUAsmWriter];

  let AssemblyParsers = [PRUAsmParser];
  let AssemblyParserVariants = [PRUAsmParserVariant];
}