//===----------------------------------------------------------------------===//
//
// This file describes the PRU instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "PRUInstrFormats.td"

//===----------------------------------------------------------------------===//
// Type Profiles.
//===----------------------------------------------------------------------===//

// Ensure the two operands of compare are of the same type
def SDT_PRUCmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
def SDT_PRUBrCC         : SDTypeProfile<0, 2, [SDTCisVT<0, OtherVT>,
                                               SDTCisVT<1, i32>]>;
def SDT_PRUSelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                               SDTCisSameAs<1, 2>, 
                                               SDTCisVT<3, i32>]>;
def SDT_PRUWrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>,
                                               SDTCisPtrTy<0>]>;
def SDT_PRUCall         : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_PRUCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i32>]>;
def SDT_PRUCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i32>, SDTCisVT<1, i32>]>;

//===----------------------------------------------------------------------===//
// PRU Specific Node Definitions.
//===----------------------------------------------------------------------===//

def PRUretFlag : SDNode<"PRUISD::RET_FLAG", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def PRUcmp     : SDNode<"PRUISD::CMP", SDT_PRUCmp, [SDNPOutGlue]>;
def PRUbrcc    : SDNode<"PRUISD::BR_CC", SDT_PRUBrCC, [SDNPHasChain,
                                                       SDNPInGlue]>;
def PRUSelectCC: SDNode<"PRUISD::SELECT_CC", SDT_PRUSelectCC, [SDNPInGlue]>;
def PRUWrapper : SDNode<"PRUISD::Wrapper", SDT_PRUWrapper>;
def PRUcall    : SDNode<"PRUISD::CALL", SDT_PRUCall,
                      [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;
def PRUcallseq_start :
                 SDNode<"ISD::CALLSEQ_START", SDT_PRUCallSeqStart,
                        [SDNPHasChain, SDNPOutGlue]>;
def PRUcallseq_end :
                 SDNode<"ISD::CALLSEQ_END",   SDT_PRUCallSeqEnd,
                        [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

//===----------------------------------------------------------------------===//
// PRU Operand Definitions.
//===----------------------------------------------------------------------===//

// Immediate/Constant operand for ALU/data-transfer instructions
def PRUimm16 : Operand<i32>, ImmLeaf<i32, [{
  return Imm >= -65535 && Imm < 65535;
}]>;

let Defs = [r2], Uses = [r2], hasSideEffects = 1, isCodeGenOnly = 1 in {
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i32imm: $amt, i32imm: $amt2), "#ADJCALLSTACKDOWN", [(PRUcallseq_start i32imm_31: $amt, i32imm_31: $amt2)]>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i32imm: $amt, i32imm: $amt2), "#ADJCALLSTACKUP", [(PRUcallseq_end i32imm_31: $amt, i32imm_31: $amt2)]>;
}

// Address operand for indexed addressing mode
def memsrc : Operand<i32> {
  let PrintMethod = "printMemSrcOperand";
  let EncoderMethod = "getMemSrcValue";
  let MIOperandInfo = (ops GPRegs, PRUimm16);
}

// Pointer memory type
def memptr : Operand<iPTR> {
  let PrintMethod = "printMemSrcOperand";
  let EncoderMethod = "getMemSrcValue";
  let MIOperandInfo = (ops GPRegs, PRUimm16);
}

// Short jump targets have OtherVT type and are printed as pcrel imm values.
def jmptarget : Operand<OtherVT> {
  let PrintMethod = "printPCRelImmOperand";
}

// Operand for printing out a condition code.
def cc : Operand<i32> {
  let PrintMethod = "printCCOperand";
}

//===----------------------------------------------------------------------===//
// PRU Complex Pattern Definitions.
//===----------------------------------------------------------------------===//

def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;

//===----------------------------------------------------------------------===//
// ALU Instructions
//===----------------------------------------------------------------------===//

let Defs = [r0] in {

multiclass ALU<bits<3> opcode, bits<4> instopcode, string opstr, SDNode opnode> {

  def rr : ALU_Inst_RR<opcode, instopcode, (outs GPRegs:$ri),
                   (ins GPRegs:$rj, GPRegs:$rk),
                   !strconcat(opstr, "\t$ri, $rj, $rk"),
                   [(set GPRegs:$ri, (opnode GPRegs:$rj, GPRegs:$rk)),
                    (implicit r0)]> {
  }
  
  def ri : ALU_Inst_RI<opcode, instopcode, (outs GPRegs:$ri),
                   (ins GPRegs:$rj, PRUimm16:$const),
                   !strconcat(opstr, "\t$ri, $rj, $const"),
                   [(set GPRegs:$ri, (opnode GPRegs:$rj, PRUimm16:$const)),
                    (implicit r0)]> {
  }
}

defm ADD  : ALU<0b000, 0b0, "add", add>;
defm SUB  : ALU<0b000, 0b10, "sub", sub>;
defm AND  : ALU<0b000, 0b1000, "and", and>;
defm OR   : ALU<0b000, 0b1001, "or",  or>;
defm NOT  : ALU<0b000, 0b1011, "not", not>;

} 

//===----------------------------------------------------------------------===//
// Data Transfer Instructions
//===----------------------------------------------------------------------===//

let isAsCheapAsAMove = 1 in {
  def LDIri  : DT_Inst_RI<0b001, 0b10,
                      (outs GPRegs:$ri), (ins PRUimm16:$const),
                      "ldi\t$ri, $const",
                      [(set GPRegs:$ri, PRUimm16:$const)]>;
} // isAsCheapAsAMove = 1


//===----------------------------------------------------------------------===//
// Comparison Instructions
//===----------------------------------------------------------------------===//

let Defs = [r0] in {

multiclass CMP_Inst<bits<3> opcode, bits<4> instopcode, string opstr> {
  
  def rr : ALU_Inst_RR<opcode, instopcode, (outs),
                   (ins GPRegs:$rj, GPRegs:$rk),
                   !strconcat(opstr, "\t$rj, $rk"),
                   [(PRUcmp GPRegs:$rj, GPRegs:$rk),
                   (implicit r0)]> {
    let ri = 0b00000;
  }
  
  def ri : ALU_Inst_RI<opcode, instopcode, (outs),
                   (ins GPRegs:$rj, PRUimm16:$const),
                   !strconcat(opstr, "\t$rj, $const"),
                   [(PRUcmp GPRegs:$rj, PRUimm16:$const),
                   (implicit r0)]> {
    let ri = 0b00000;
  }
}

defm CMP  : CMP_Inst<0b01, "cmp">;

} 

//===----------------------------------------------------------------------===//
// Branch Instructions
//===----------------------------------------------------------------------===//

// Unconditional jump
let isBranch = 1, isTerminator = 1, isBarrier = 1 in {
  def JAL : FC_Inst<0b001, 0b1,
                    (outs), (ins jmptarget:$addr),
                    "jal\t$addr",
                    [(br bb:$addr)]> {
  let ri = 0b00000;
  let control = 0b1;
}

} // isBranch = 1, isTerminator = 1, isBarrier = 1

let isTerminator = 1, isReturn = 1, isBarrier = 1 in {
  def JMP : InstPRU<(outs GPRegs:$ri), (ins),
                    "jmp\t$ri",  [(PRUretFlag)]> {
    let Opcode = 0b001;
    let InstOpcode = 0b0;
    let Inst{26-0}  = 0;
  }
}

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

let Defs = [r2], Uses = [r2] in {
def ADJCALLSTACKDOWN : PRUPseudoInst<(outs), (ins PRUimm16:$amt),
                                     "#ADJCALLSTACKDOWN",
                                     [(PRUcallseq_start timm:$amt)]>;
def ADJCALLSTACKUP : PRUPseudoInst<(outs), (ins PRUimm16:$amt1, PRUimm16:$amt2),
                                   "#ADJCALLSTACKUP",
                                   [(PRUcallseq_end timm:$amt1, timm:$amt2)]>;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//
let usesCustomInserter = 1 in
def Select : PRUPseudoInst<(outs GPRegs:$dst),
                    (ins GPRegs:$src, GPRegs:$src2, i32imm:$condition),
                    "# Select PSEUDO",
                    [(set GPRegs:$dst,
                     (PRUSelectCC GPRegs:$src, GPRegs:$src2, imm:$condition))]>;

// GlobalAddress, ExternalSymbol
def : Pat<(i32 (PRUWrapper tglobaladdr:$dst)), (LDIri tglobaladdr:$dst)>;
def : Pat<(i32 (PRUWrapper texternalsym:$dst)), (LDIri texternalsym:$dst)>;

def : Pat<(add GPRegs:$src, (PRUWrapper tglobaladdr:$src2)),
          (ADDri GPRegs:$src, tglobaladdr:$src2)>;
def : Pat<(add GPRegs:$src, (PRUWrapper texternalsym:$src2)),
          (ADDri GPRegs:$src, texternalsym:$src2)>;

