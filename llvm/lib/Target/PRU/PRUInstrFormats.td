//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//
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
  let Inst{31-29} = Opcode;
  
  bits<4> InstOpcode = 0;
  let Inst{28-25} = InstOpcode;
}

// PRU pseudo instructions format
class PRUPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

//===----------------------------------------------------------------------===//
// ALU Instructions
//===----------------------------------------------------------------------===//

// ALU register-register instruction
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
  let Inst{23-21} = rfk;
  let Inst{20-16} = rk;
  let Inst{15-13} = rfj;
  let Inst{12-8} = rj;
  let Inst{7-5} = rfi;
  let Inst{4-0} = ri;
}

// ALU register-immediate instruction
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
  let Inst{23-16} = imm;
  let Inst{15-13} = rfj;
  let Inst{12-8} = rj;
  let Inst{5-5} = rfi;
  let Inst{4-0} = ri;
}

//===----------------------------------------------------------------------===//
// Data Transfer Instructions
//===----------------------------------------------------------------------===//

// Data Transfer register-register instruction
class DT_Inst_RR<bits<3> opcode, bits<4> instopcode, dag outs, dag ins, string asmstr,
                  list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {

  bits<5> ri; // destination register
  bits<3> rfi; // destination field
  bits<3> rfj; // source 1 field
  bits<5> rj; // source 1 register

  let Opcode = opcode;
  let InstOpcode = instopcode;
  let Inst{24-24} = 0;
  let Inst{23-21} = rfj;
  let Inst{20-16} = rj;
  let Inst{15-8} = 0; //reserved
  let Inst{7-5} = rfi;
  let Inst{4-0} = ri;
}

// Data Transfer register-immediate instruction
class DT_Inst_RI<bits<3> opcode, bits<4> instopcode, dag outs, dag ins, string asmstr,
                  list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {

  bits<5> ri; // destination register
  bits<3> rfi; // destination field
  bits<8> imm; // source 2 (immediate)

  let Opcode = opcode;
  let InstOpcode = instopcode;
  let Inst{24-24} = 1;
  let Inst{23-8} = imm;
  let Inst{7-5} = rfi;
  let Inst{4-0} = ri;
}

//===----------------------------------------------------------------------===//
// Flow Control Instructions
//===----------------------------------------------------------------------===//

class FC_Inst<bits<3> opcode, bits<4> instopcode, dag outs, dag ins, string asmstr,
                list<dag> pattern>
    : InstPRU<outs, ins, asmstr, pattern> {

  bits<5> ri; // destination register
  bits<3> rfi; // destination field
  bits<3> rfj; // source 1 field
  bits<5> rj; // source 1 register

  let Opcode = opcode;
  let InstOpcode = instopcode;
  let Inst{24-24} = 0;
  let Inst{23-21} = rfj;
  let Inst{20-16} = rj;
  let Inst{15-8} = 0; //reserved
  let Inst{7-5} = rfi;
  let Inst{4-0} = ri;
}
