//===----------------------------------------------------------------------===//
//  PRU Instruction Format Definitions.
//===----------------------------------------------------------------------===//

// Generic PRU Format
class InstPRU<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;

  let Namespace = "PRU";
  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString   = asmstr;
  let Pattern = pattern;
  let Size = 4;

  bits<3> Opcode = 0;
  let Inst{29-31} = Opcode;
  
  bits<4> InstOpcode = 0;
  let Inst{25-28} = InstOpcode;

  let DecoderNamespace = "PRU";
  field bits<32> SoftFail = 0;

  }

// PRU Pseudo Instructions Format

class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {
  let Pattern = pattern;
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

// ALU Instructions

//===----------------------------------------------------------------------===//
// ALU register-register instruction
//===----------------------------------------------------------------------===//
class ALU_Inst_RR<bits<3> opcode, bits<4> instopcode, dag outs, dag ins, string asmstr,
                  list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {

  bits<5> ri; // destination register
  bits<5> rj; // source 1 register
  bits<5> rk; // source 2 register
  bits<3> rfi; // destination field
  bits<3> rfj; // source 1 field
  bits<3> rfk; // source 2 field

  let Opcode = opcode;
  let InstOpcode = instopcode;
  let Inst{24-24} = 1;
  let Inst{21-23} = rfk;
  let Inst{16-20} = rk;
  let Inst{13-15} = rfj;
  let Inst{8-12} = rj;
  let Inst{5-7} = rfi;
  let Inst{0-4} = ri;
}

//===----------------------------------------------------------------------===//
// ALU register-immediate instruction
//===----------------------------------------------------------------------===//

class ALU_Inst_RI<bits<3> opcode, bits<4> instopcode, dag outs, dag ins, string asmstr,
                  list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {

  bits<5> ri; // destination register
  bits<5> rj; // source 1 register
  bits<8> imm; // source 2 (immediate)
  bits<3> rfi; // destination field
  bits<3> rfj; // source 1 field

  let Opcode = opcode;
  let InstOpcode = instopcode;
  let Inst{24-24} = 0;
  let Inst{16-23} = imm;
  let Inst{13-15} = rfj;
  let Inst{8-12} = rj;
  let Inst{5-7} = rfi;
  let Inst{0-4} = ri;
}

// Dummy instruction, need to be changed for the below two.
// Flow Control Instructions
class FC_Inst<bits<8> opcode, dag outs, dag ins, string asmstr,
                list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {

  bits<5> ri;
  bits<5> rj;
  bits<3> rfi;
  bits<3> rfj;

  let Opcode = opcode;
  let Inst{27-31} = ri;
  let Inst{24-26} = rfi;
  let Inst{19-23} = rj;
  let Inst{16-18} = rfj;
  let Inst{13-10} = condition; 
  let Inst{9-0} = addr; 
  let Opcode = opcode;
}

// Load/Store Instructions
class LS_Inst<bits<8> opcode, dag outs, dag ins, string asmstr,
                list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {

  bits<5> ri;
  bits<20> addr;

  let Opcode = opcode;
  let Inst{23-19} = ri;
  let Inst{19-0} = addr;
}