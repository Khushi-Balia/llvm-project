//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for PRU architecture.
//
//===----------------------------------------------------------------------===//

// r5...r13 are callee-saved registers
def CSR_LP32 : CalleeSavedRegs<(sequence "r%u", 5, 13)>;