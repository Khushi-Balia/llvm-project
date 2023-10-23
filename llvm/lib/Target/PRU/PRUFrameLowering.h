//===----------------------------------------------------------------------===//
//
// This class implements PRU-specific bits of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef PRUFRAMEINFO_H
#define PRUFRAMEINFO_H

#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {
class PRUSubtarget;

class PRUFrameLowering : public TargetFrameLowering {
public:
  PRUFrameLowering(const PRUSubtarget &STI);

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code 
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  bool spillCalleeSavedRegisters(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator MI,
                                 const std::vector<CalleeSavedInfo> &CSI,
                                 const TargetRegisterInfo *TRI) const override;
  
  bool restoreCalleeSavedRegisters(MachineBasicBlock &MBB,
                                  MachineBasicBlock::iterator MI,
                                  const std::vector<CalleeSavedInfo> &CSI,
                                  const TargetRegisterInfo *TRI) const override;

  bool hasFP(const MachineFunction &MF) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;
  
  void processFunctionBeforeFrameFinalized(MachineFunction &MF,
                                     RegScavenger *RS = nullptr) const override;

  static int stackSlotSize() { return 4; } 
};
}

#endif
