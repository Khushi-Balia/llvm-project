#include "PRUMCAsmInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Triple.h"
using namespace llvm;

void PRUMCAsmInfo::anchor() {}

PRUMCAsmInfo::PRUMCAsmInfo(const Triple &TT) {
  CommentString = "//";
  SupportsDebugInformation = true;
  PrivateLabelPrefix = "";
}
