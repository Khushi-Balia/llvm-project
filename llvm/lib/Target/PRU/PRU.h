//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM PRU back-end.
//
//===----------------------------------------------------------------------===//

#ifndef TARGET_PRU_H
#define TARGET_PRU_H

#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace PRUCC {
  enum CondCodes {
    COND_U      = 0,    // unconditional
    COND_INVALID = -1
  };
}

namespace llvm {
class PRUTargetMachine;
class MCInst;
class MachineInstr;

FunctionPass *createPRUISelDag(PRUTargetMachine &TM,
                               CodeGenOpt::Level OptLevel);

} // end namespace llvm;

#endif
