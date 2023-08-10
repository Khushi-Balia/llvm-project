//===----------------------------------------------------------------------===//
//
// This file contains the PRU implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef PRUINSTRINFO_H_INCLUDED
#define PRUINSTRINFO_H_INCLUDED

#include "PRURegisterInfo.h"
#include "PRU.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "PRUGenInstrInfo.inc"

namespace llvm {

class PRUInstrInfo : public PRUGenInstrInfo {
virtual void anchor();
public:
  explicit PRUInstrInfo();

  const PRURegisterInfo &getRegisterInfo() const { return RI; }

// Return the number of bytes of code the specified instruction maybe.
  unsigned GetInstSizeInBytes(const MachineInstr &MI) const;

  private:
  const PRURegisterInfo RI;
 
};

} // namespace llvm

#endif