//===----------------------------------------------------------------------===//
//
// Implements the info about PRU target spec.
//
//===----------------------------------------------------------------------===//

#include "PRUTargetMachine.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {

  std::string dataLayout = "";

  dataLayout += "e"; 
  dataLayout += "-m:e"; 
  dataLayout += "-p:32:32"; 
  dataLayout += "-i1:8:32"; 
  dataLayout += "-i8:8:32"; 
  dataLayout += "-i16:16:32"; 
  dataLayout += "-i64:32"; 
  dataLayout += "-f64:32"; 
  dataLayout += "-a:0:32"; 
  dataLayout += "-n32"; // Set native integer width to 32-bits
  
  // "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32"

  return dataLayout;
}

static Reloc::Model getEffectiveRelocModel(const Triple &TT,
                                           Optional<Reloc::Model> RM) {
  if (!RM.hasValue())

    return TT.isOSBinFormatMachO() ? Reloc::PIC_ : Reloc::Static;

  if (*RM == Reloc::DynamicNoPIC && !TT.isOSDarwin())
    return Reloc::Static;

  return *RM;
}

PRUTargetMachine::PRUTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   CodeModel::Model CM, CodeGenOpt::Level OL)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(TT, RM), CM, OL),
      Subtarget(TT, CPU, FS, *this),
      TLOF(make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

namespace {
class PRUPassConfig : public TargetPassConfig {
public:
  PRUPassConfig(PRUTargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  PRUTargetMachine &getPRUTargetMachine() const {
    return getTM<PRUTargetMachine>();
  }

  virtual bool addInstSelector() override;
};
}

TargetPassConfig *PRUTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new PRUPassConfig(this, PM);
}

bool PRUPassConfig::addInstSelector() {
  addPass(createPRUISelDag(getPRUTargetMachine(), getOptLevel()));
  return false;
}

// Force static initialization.
extern "C" void LLVMInitializePRUTarget() {
  RegisterTargetMachine<PRUTargetMachine> X(getThePRUTarget());
}
