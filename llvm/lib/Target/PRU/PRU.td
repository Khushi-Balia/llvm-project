//===----------------------------------------------------------------------===//
// This is the top level entry point for the PRU target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Target-dependent interfaces
//===----------------------------------------------------------------------===//

// Register Infomation
include "PRURegisterInfo.td"

// Instruction Description
include "PRUInstrInfo.td"

// Calling Convention
include "PRUCallingConv.td"

// Devices Definitions
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