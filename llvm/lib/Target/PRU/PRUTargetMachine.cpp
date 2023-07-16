#include "PRUTargetMachine.h"

#include "PRU.h"
#include "PRUTargetObjectFile.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
#include "TargetInfo/PRUTargetInfo.h"


#include <optional>
#include <string>

namespace llvm {
static StringRef computeDataLayout(const Triple &TT, StringRef CPU,
                                   const TargetOptions &Options)
 { return "e-m:e-p:32:8-i8:8-i16:16:8-i32:32:8-i64:64:8-f32:8-f64:8-n8";
                                   }

static StringRef getCPU(StringRef CPU) {
  if (CPU.empty() || CPU == "generic") {
    return "pru";
  }

  return CPU;
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

PRUTargetMachine::PRUTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   std::optional<Reloc::Model> RM,
                                   std::optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, getCPU(CPU), Options), TT, CPU, FS, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      Subtarget(TT, std::string(getCPU(CPU)), std::string(FS), *this) {
  this->TLOF = std::make_unique<PRUTargetObjectFile>();
  initAsmInfo();
  }

PRUTargetMachine::~PRUTargetMachine() {}

namespace {
/// PRU Code Generator Pass Configuration Options.
class PRUPassConfig : public TargetPassConfig {
public:
  PRUPassConfig(PRUTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  PRUTargetMachine &getPRUTargetMachine() const {
    return getTM<PRUTargetMachine>();
  }
};
}
PRUSubtarget const *
PRUTargetMachine::getSubtargetImpl(Function const &F) const {
  return &Subtarget;
}

PRUSubtarget const *PRUTargetMachine::getSubtargetImpl() const {
  return &Subtarget;
}

TargetPassConfig *PRUTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new PRUPassConfig(*this, PM);
}

// TODO FIXME (implement PRUMachineFunctionInfo class)
MachineFunctionInfo *PRUTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return nullptr;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializePRUTarget() {
// Register the target.
RegisterTargetMachine<PRUTargetMachine> X(getThePRUTarget());}
}