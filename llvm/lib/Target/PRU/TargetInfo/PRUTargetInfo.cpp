#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getThePRUTarget() {
  static Target ThePRUTarget;
  return ThePRUTarget;
}

extern "C" void LLVMInitializePRUTargetInfo() {
  RegisterTarget<Triple::pru> X(getThePRUTarget(), "pru", "PRU");
}
