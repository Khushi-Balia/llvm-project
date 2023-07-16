//Dummy stuff

#ifndef LLVM_PRU_TARGET_MACHINE_H
#define LLVM_PRU_TARGET_MACHINE_H

#include "llvm/Target/TargetMachine.h"
#include "llvm/IR/DataLayout.h"

#include "PRUInstrInfo.h"
#include "PRUSubtarget.h"
#include "PRUFrameLowering.h"
#include "PRUISelLowering.h"

#include <memory>

#include <optional>

namespace llvm {

/// A generic PRU implementation.
class PRUTargetMachine : public LLVMTargetMachine {

  private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  PRUSubtarget Subtarget;

public:
  PRUTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   std::optional<Reloc::Model> RM,
                   std::optional<CodeModel::Model> CM, CodeGenOpt::Level OL,
                   bool JIT);

  const PRUSubtarget *getSubtargetImpl() const;
  const PRUSubtarget *getSubtargetImpl(const Function &) const override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return this->TLOF.get();
  }

  ~PRUTargetMachine();

  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  MachineFunctionInfo *
  createMachineFunctionInfo(BumpPtrAllocator &Allocator, const Function &F,
                            const TargetSubtargetInfo *STI) const override;
  
};

} // end namespace llvm

#endif