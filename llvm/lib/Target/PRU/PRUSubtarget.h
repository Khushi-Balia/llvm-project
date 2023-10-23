//===----------------------------------------------------------------------===//
//
// This file declares the PRU specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef PRUSUBTARGET_H
#define PRUSUBTARGET_H

#include "PRUFrameLowering.h"
#include "PRUISelLowering.h"
#include "PRUInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetSubtargetInfo.h"

#define GET_SUBTARGETINFO_HEADER
#include "PRUGenSubtargetInfo.inc"

namespace llvm {
class StringRef;

class PRUSubtarget : public PRUGenSubtargetInfo {
  virtual void anchor();

  PRUInstrInfo InstrInfo;
  PRUFrameLowering FrameLowering;
  PRUTargetLowering TLInfo;

public:
  // Initializes the data members to match that of the specified triple.
  PRUSubtarget(const Triple &TT, StringRef CPU,
               StringRef FS, PRUTargetMachine &TM);

  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);

  const PRUFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }
  const PRUInstrInfo *getInstrInfo() const override {
  	return &InstrInfo;
  }
  const TargetRegisterInfo *getRegisterInfo() const override {
    return &InstrInfo.getRegisterInfo();
  }
  const PRUTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }
};
} // End llvm namespace

#endif
