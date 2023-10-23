//===----------------------------------------------------------------------===//
//
// This file declares the PRU specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef PRUTARGETMACHINE_H
#define PRUTARGETMACHINE_H

#include "PRU.h"
#include "PRUSubtarget.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {

class PRUTargetMachine : public LLVMTargetMachine {
  PRUSubtarget Subtarget;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;

public:
  PRUTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                   StringRef FS, const TargetOptions &Options,
                   Optional<Reloc::Model> RM, CodeModel::Model CM,
                   CodeGenOpt::Level OL);

  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM) override;
  
  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  virtual const TargetSubtargetInfo *
  getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }
  
};

} // end namespace llvm

#endif
