#include "PRUTargetMachine.h"
#include "PRU.h"
#include "PRUTargetMachine.h"
#include "PRUSubtarget.h"
#include "MCTargetDesc/PRUMCTargetDesc.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define PASS_NAME "PRU DAG->DAG Instruction Selection"

namespace {
  
class PRUDAGToDAGISel : public SelectionDAGISel {
public:
  static char ID;

 PRUDAGToDAGISel() = delete;
 explicit PRUDAGToDAGISel(PRUTargetMachine &tm) : SelectionDAGISel(ID, tm), Subtarget(nullptr) {}

 bool runOnMachineFunction(MachineFunction &MF) override;

 bool SelectAddr(SDNode *Op, SDValue N, SDValue &Base, SDValue &Disp);

 #include "PRUGenDAGISel.inc"

private:
 void Select(SDNode *N) override;
 bool trySelect(SDNode *N);

 const PRUSubtarget *Subtarget;

  };
} //namespace

bool PRUDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget = &MF.getSubtarget<PRUSubtarget>();
  return SelectionDAGISel::runOnMachineFunction(MF);
}

void PRUDAGToDAGISel::Select(SDNode *N) {

    if (N->isMachineOpcode()) {
    LLVM_DEBUG(errs() << "== "; N->dump(CurDAG); errs() << "\n");
    N->setNodeId(-1);
    return;
  }

  // See if subclasses can handle this node.
  if (trySelect(N))
    return;

  // Select the default instruction
  SelectCode(N);
}
