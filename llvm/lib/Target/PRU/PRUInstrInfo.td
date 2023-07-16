include "PRUInstrFormats.td"

//===----------------------------------------------------------------------===//
// PRU Type Profiles
//===----------------------------------------------------------------------===//

def SDT_PRUCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>]>;
def SDT_PRUCallSeqEnd : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;
def SDT_PRUJmpLink : SDTypeProfile<0, 1, [SDTCisVT<0, iPTR>]>;
def SDT_PRUCall : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_PRUSelectCC : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>, SDTCisSameAs<1, 2>, SDTCisVT<3, i32>]>;

//===----------------------------------------------------------------------===//
// PRU Specific Node Definitions
//===----------------------------------------------------------------------===//

def PRUCall: SDNode<"PRUISD::CALL", SDT_PRUCall,
                  [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue, SDNPVariadic]>;
def PRURet: SDNode<"PRUISD::RET_FLAG", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;                  
def PRUJmpLink : SDNode<"PRUISD::JmpLink", SDT_PRUJmpLink,
                         [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue,
                          SDNPVariadic]>;
def PRUcallseq_start : SDNode<"ISD::CALLSEQ_START", SDT_PRUCallSeqStart,
                           [SDNPHasChain, SDNPSideEffect, SDNPOutGlue]>;
def PRUcallseq_end : SDNode<"ISD::CALLSEQ_END", SDT_PRUCallSeqEnd,
                         [SDNPHasChain, SDNPOptInGlue,
                          SDNPSideEffect, SDNPOutGlue]>;
def PRUselectcc : SDNode<"PRUISD::SELECT_CC", SDT_PRUSelectCC, [SDNPInGlue]>;

//===----------------------------------------------------------------------===//
// Pattern fragments.
//===----------------------------------------------------------------------===//

def i8imm_31 : ImmLeaf<i8, [{return (uint8_t) Imm <= 31;}]>;
def i16imm_31 : ImmLeaf<i16, [{return (uint16_t) Imm <= 31;}]>;
def i32imm_31 : ImmLeaf<i32, [{return (uint32_t) Imm <= 31;}]>;

def i8imm_255 : ImmLeaf<i8, [{return (uint8_t) Imm <= 255;}]>;
def i16imm_255 : ImmLeaf<i16, [{return (uint16_t) Imm <= 255;}]>;
def i32imm_255 : ImmLeaf<i32, [{return (uint32_t) Imm <= 255;}]>;

def i16imm_65535 : ImmLeaf<i16, [{return (uint16_t) Imm <= 65535;}]>;
def i32imm_65535 : ImmLeaf<i32, [{return (uint32_t) Imm <= 65535;}]>;

let Defs = [r2], Uses = [r2], hasSideEffects = 1, isCodeGenOnly = 1 in {
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm: $amt, i32imm: $amt2), "#ADJCALLSTACKDOWN", [(PRUcallseq_start i32imm_31: $amt, i32imm_31: $amt2)]>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm: $amt, i32imm: $amt2), "#ADJCALLSTACKUP", [(PRUcallseq_end i32imm_31: $amt, i32imm_31: $amt2)]>;
}

//===----------------------------------------------------------------------===//
// PRU Operand Definitions
//===----------------------------------------------------------------------===//

//jump targets have OtherVT type
def jump_target : Operand<OtherVT> 
{ let OperandType = "OPERAND_PCREL"; }

def gaddr : Operand<iPTR>;

// PRU Complex Pattern Definitions.

def addr : Operand<iPTR>, ComplexPattern<iPTR, 2, "SelectAddress"> {
    let MIOperandInfo = (ops reg32: $base, i32imm: $offset);
    let PrintMethod = "print_addr";
}

def regaddr : Operand<iPTR>, ComplexPattern<iPTR, 2, "SelectAddressReg"> {
    let MIOperandInfo = (ops reg32: $base, reg32: $offset);
    let PrintMethod = "print_addr";
}

// Instructions specific format

// Arithmetic and logical instructions with 3 register operands
multiclass ArithLogicRI<bits<8> opcode, string opstr, SDNode opNode>
{
  
  def RR: ALU_Inst_RR<opcode, (outs reg32:$ri), (ins reg32:$rj, reg32:$rk),
       !strconcat(opstr, "\t$ri, $rj, $rk"),
       [(set reg32:$ri, (opNode reg32:$rj, reg32:$rk))]>;


// Arithmetic and logical instructions with 2 register operands

 def RI : ALU_Inst_RI<opcode, (outs reg32:$ri), (ins reg32:$rj, i16imm_255:$const),
       !strconcat(opstr, "\t$ri, $rj, $const"),
       [(set reg32:$ri, (opNode reg32:$rj, i16imm_255:$const))]>;
}

//Load and store
multiclass LDI<bits<8> opcode, string opstr, SDNode opNode, Operand od>{
  def LD : LS_Inst<opcode, (outs reg32:$ri), (ins od:$addr),
         !strconcat(opstr, "\t$ri, $addr"),
       [(set reg32:$ri, (opNode addr:$addr))]>;
}
