#ifndef PRUTARGETSTREAMER_H_INCLUDED
#define PRUTARGETSTREAMER_H_INCLUDED

#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/BinaryFormat/ELF.h"

namespace llvm {

class PRUTargetStreamer : public MCTargetStreamer {
public:
  explicit PRUTargetStreamer(MCStreamer &S);
};

// Assembly text streamer
class PRUTargetAsmStreamer : public PRUTargetStreamer {
  formatted_raw_ostream &OS;

public:
  PRUTargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);
};

} // namespace llvm

#endif 