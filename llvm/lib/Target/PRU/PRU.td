//===----------------------------------------------------------------------===//
//
// This is the top level entry point for the PRU target.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Descriptions
//===----------------------------------------------------------------------===//

include "PRURegisterInfo.td"
include "PRUCallingConv.td"
include "PRUInstrInfo.td"

def PRUInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// PRU processors supported.
//===----------------------------------------------------------------------===//

class Proc<string Name, list<SubtargetFeature> Features>
    : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def PRU : Target {
  let InstructionSet = PRUInstrInfo;
}
