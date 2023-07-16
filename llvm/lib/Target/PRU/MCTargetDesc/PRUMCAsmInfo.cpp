#include "PRUMCAsmInfo.h"
#include "llvm/ADT/Triple.h"

using namespace llvm;

namespace llvm {
PRUMCAsmInfo::PRUMCAsmInfo(const Triple &T, const MCTargetOptions &Options) {
  AlignmentIsInBytes = true;
  Data16bitsDirective = "\t.word\t";
  PrivateGlobalPrefix = "$";
  PrivateLabelPrefix = "$";
  CommentString = "#";
  SupportsDebugInformation = true;
  DwarfRegNumForCFI = true;
}
} // end of namespace llvm