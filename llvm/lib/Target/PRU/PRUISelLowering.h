#ifndef PRUISELLOWERING_H_INCLUDED
#define PRUISELLOWERING_H_INCLUDED

#include "PRU.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

namespace PRUISD {

/// PRU Specific DAG Nodes
enum NodeType {
/// Start the numbering where the builtin ops leave off.
  FIRST_NUMBER = ISD::BUILTIN_OP_END,

ADD,                      
ADC,                     
SUB,                      
SUC,                      
LSL,                      
LSR,                      
RSB,                      
RSC,                      
AND,                      
OR ,                      
XOR,                      
NOT,                      
MIN,                      
MAX,                      
CLR,                      
SET ,                     
LDI ,                     
LBBO ,                    
LBCO ,                   
SBBO ,                    
SBCO ,                    
LFC ,                    
STC ,                    
JAL ,                    
JMP ,                     
QBGT ,                    
QBLT ,                    
QBEQ ,                    
QBGE ,                    
QBLE ,                    
QBNE ,                    
QBA ,                     
QBBS ,                    
QBBC ,                    
LMBD ,                    
CALL ,                    
WBC ,                    
WBS ,                     
MOV ,                     
MVIB ,                    
MVIW ,                    
MVID ,                    
SCAN ,                    
HALT ,                    
SLP ,                    
RET ,                     
ZERO ,                    
FILL ,                    
XIN ,                    
XOUT ,                    
XCHG ,                    
SXIN ,                    
SXOUT ,                   
SXCHG ,                   
LOOP  ,                   
ILOOP ,                                       
MAXIDX    
};

} // end of namespace PRUISD

class PRUSubtarget;
class PRUTargetMachine;

class PRUTargetLowering : public TargetLowering {

public:
  PRUTargetLowering(const PRUTargetMachine &TM,
                         const PRUSubtarget &STI);

  const char *getTargetNodeName(unsigned Opcode) const override;

  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               SmallVectorImpl<ISD::InputArg> const &Ins,
                               SDLoc const &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      SmallVectorImpl<ISD::OutputArg> const &Outs,
                      SmallVectorImpl<SDValue> const &OutVals, SDLoc const &dl,
                      SelectionDAG &DAG) const override;


  protected:
  const PRUSubtarget &Subtarget;
};

} // namespace llvm

#endif