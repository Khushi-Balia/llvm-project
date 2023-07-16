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

  // TODO
  const PRURegisterInfo &getRegisterInfo() const { return RI; }

  private:
  const PRURegisterInfo RI;
 
};

} // namespace llvm

#endif