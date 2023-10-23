#ifndef PRUFIXUPKINDS_H
#define PRUFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace PRU {

  enum Fixups {
    fixup_PRU_NONE = FirstTargetFixupKind,
    fixup_PRU_32,
    LastTargetFixupKind,
    NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
  };
} // namespace PRU
} // namespace llvm


#endif
