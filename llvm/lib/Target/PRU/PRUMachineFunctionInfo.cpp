#include "PRUMachineFunctionInfo.h"

#include "PRUInstrInfo.h"
#include "PRUSubtarget.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"

using namespace llvm;

bool FixGlobalBaseReg;

PRUMachineFunctionInfo::~PRUMachineFunctionInfo() { }

void PRUMachineFunctionInfo::anchor() { }