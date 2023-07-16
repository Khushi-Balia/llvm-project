#ifndef LLVM_PRU_SELECTION_DAG_INFO_H
#define LLVM_PRU_SELECTION_DAG_INFO_H

#include "llvm/CodeGen/SelectionDAGTargetInfo.h"

namespace llvm {

/// Holds information about the PRU instruction selection DAG.
class PRUSelectionDAGInfo : public SelectionDAGTargetInfo {
public:
};

} // end namespace llvm

#endif // LLVM_PRU_SELECTION_DAG_INFO_H