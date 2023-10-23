//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the PRUMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef PRUTARGETASMINFO_H
#define PRUTARGETASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class PRUMCAsmInfo : public MCAsmInfoELF {
  virtual void anchor();

public:
  explicit PRUMCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
