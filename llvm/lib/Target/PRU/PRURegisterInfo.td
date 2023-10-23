//===----------------------------------------------------------------------===//
//  Declarations that describe the PRU register file 
//===----------------------------------------------------------------------===//
 
// 32-bit General purpose register definition
class PRUReg<bits<16> Enc, string n, list<Register> subregs = [], list<string> altNames = []> : RegisterWithSubRegs<name, subregs> {
  field bits<16> Num = Enc;
 
  let HWEncoding = Enc;
  let Namespace = "PRU";
  let SubRegs = subregs;
  let AltNames = altNames;
}
 
// Subregister indices
let Namespace = "PRU" in {
  def sub_8_0 : SubRegIndex<8>;
  def sub_8_8 : SubRegIndex<8, 8>;
  def sub_16_0 : SubRegIndex<16>;
  def sub_16_16 : SubRegIndex<16, 16>;
 
  def sub_8_16 : ComposedSubRegIndex<sub_16_16, sub_8_0>;
  def sub_8_24 : ComposedSubRegIndex<sub_16_16, sub_8_8>;
}
 
//===----------------------------------------------------------------------===//
//  32-bit Genral purpose registers
//===----------------------------------------------------------------------===//
 
//// 8 bit subregs
def r0_b0 : PRUReg<0, "r0.b0">;
def r0_b1 : PRUReg<0, "r0.b1">;
def r0_b2 : PRUReg<0, "r0.b2">;
def r0_b3 : PRUReg<0, "r0.b3">;
def r1_b0 : PRUReg<1, "r1.b0">;
def r1_b1 : PRUReg<1, "r1.b1">;
def r1_b2 : PRUReg<1, "r1.b2">;
def r1_b3 : PRUReg<1, "r1.b3">;
def r2_b0 : PRUReg<2, "r2.b0">;
def r2_b1 : PRUReg<2, "r2.b1">;
def r2_b2 : PRUReg<2, "r2.b2">;
def r2_b3 : PRUReg<2, "r2.b3">;
def r3_b0 : PRUReg<3, "r3.b0">;
def r3_b1 : PRUReg<3, "r3.b1">;
def r3_b2 : PRUReg<3, "r3.b2">;
def r3_b3 : PRUReg<3, "r3.b3">;
def r4_b0 : PRUReg<4, "r4.b0">;
def r4_b1 : PRUReg<4, "r4.b1">;
def r4_b2 : PRUReg<4, "r4.b2">;
def r4_b3 : PRUReg<4, "r4.b3">;
def r5_b0 : PRUReg<5, "r5.b0">;
def r5_b1 : PRUReg<5, "r5.b1">;
def r5_b2 : PRUReg<5, "r5.b2">;
def r5_b3 : PRUReg<5, "r5.b3">;
def r6_b0 : PRUReg<6, "r6.b0">;
def r6_b1 : PRUReg<6, "r6.b1">;
def r6_b2 : PRUReg<6, "r6.b2">;
def r6_b3 : PRUReg<6, "r6.b3">;
def r7_b0 : PRUReg<7, "r7.b0">;
def r7_b1 : PRUReg<7, "r7.b1">;
def r7_b2 : PRUReg<7, "r7.b2">;
def r7_b3 : PRUReg<7, "r7.b3">;
def r8_b0 : PRUReg<8, "r8.b0">;
def r8_b1 : PRUReg<8, "r8.b1">;
def r8_b2 : PRUReg<8, "r8.b2">;
def r8_b3 : PRUReg<8, "r8.b3">;
def r9_b0 : PRUReg<9, "r9.b0">;
def r9_b1 : PRUReg<9, "r9.b1">;
def r9_b2 : PRUReg<9, "r9.b2">;
def r9_b3 : PRUReg<9, "r9.b3">;
def r10_b0 : PRUReg<10, "r10.b0">;
def r10_b1 : PRUReg<10, "r10.b1">;
def r10_b2 : PRUReg<10, "r10.b2">;
def r10_b3 : PRUReg<10, "r10.b3">;
def r11_b0 : PRUReg<11, "r11.b0">;
def r11_b1 : PRUReg<11, "r11.b1">;
def r11_b2 : PRUReg<11, "r11.b2">;
def r11_b3 : PRUReg<11, "r11.b3">;
def r12_b0 : PRUReg<12, "r12.b0">;
def r12_b1 : PRUReg<12, "r12.b1">;
def r12_b2 : PRUReg<12, "r12.b2">;
def r12_b3 : PRUReg<12, "r12.b3">;
def r13_b0 : PRUReg<13, "r13.b0">;
def r13_b1 : PRUReg<13, "r13.b1">;
def r13_b2 : PRUReg<13, "r13.b2">;
def r13_b3 : PRUReg<13, "r13.b3">;
def r14_b0 : PRUReg<14, "r14.b0">;
def r14_b1 : PRUReg<14, "r14.b1">;
def r14_b2 : PRUReg<14, "r14.b2">;
def r14_b3 : PRUReg<14, "r14.b3">;
def r15_b0 : PRUReg<15, "r15.b0">;
def r15_b1 : PRUReg<15, "r15.b1">;
def r15_b2 : PRUReg<15, "r15.b2">;
def r15_b3 : PRUReg<15, "r15.b3">;
def r16_b0 : PRUReg<16, "r16.b0">;
def r16_b1 : PRUReg<16, "r16.b1">;
def r16_b2 : PRUReg<16, "r16.b2">;
def r16_b3 : PRUReg<16, "r16.b3">;
def r17_b0 : PRUReg<17, "r17.b0">;
def r17_b1 : PRUReg<17, "r17.b1">;
def r17_b2 : PRUReg<17, "r17.b2">;
def r17_b3 : PRUReg<17, "r17.b3">;
def r18_b0 : PRUReg<18, "r18.b0">;
def r18_b1 : PRUReg<18, "r18.b1">;
def r18_b2 : PRUReg<18, "r18.b2">;
def r18_b3 : PRUReg<18, "r18.b3">;
def r19_b0 : PRUReg<19, "r19.b0">;
def r19_b1 : PRUReg<19, "r19.b1">;
def r19_b2 : PRUReg<19, "r19.b2">;
def r19_b3 : PRUReg<19, "r19.b3">;
def r20_b0 : PRUReg<20, "r20.b0">;
def r20_b1 : PRUReg<20, "r20.b1">;
def r20_b2 : PRUReg<20, "r20.b2">;
def r20_b3 : PRUReg<20, "r20.b3">;
def r21_b0 : PRUReg<21, "r21.b0">;
def r21_b1 : PRUReg<21, "r21.b1">;
def r21_b2 : PRUReg<21, "r21.b2">;
def r21_b3 : PRUReg<21, "r21.b3">;
def r22_b0 : PRUReg<22, "r22.b0">;
def r22_b1 : PRUReg<22, "r22.b1">;
def r22_b2 : PRUReg<22, "r22.b2">;
def r22_b3 : PRUReg<22, "r22.b3">;
def r23_b0 : PRUReg<23, "r23.b0">;
def r23_b1 : PRUReg<23, "r23.b1">;
def r23_b2 : PRUReg<23, "r23.b2">;
def r23_b3 : PRUReg<23, "r23.b3">;
def r24_b0 : PRUReg<24, "r24.b0">;
def r24_b1 : PRUReg<24, "r24.b1">;
def r24_b2 : PRUReg<24, "r24.b2">;
def r24_b3 : PRUReg<24, "r24.b3">;
def r25_b0 : PRUReg<25, "r25.b0">;
def r25_b1 : PRUReg<25, "r25.b1">;
def r25_b2 : PRUReg<25, "r25.b2">;
def r25_b3 : PRUReg<25, "r25.b3">;
def r26_b0 : PRUReg<26, "r26.b0">;
def r26_b1 : PRUReg<26, "r26.b1">;
def r26_b2 : PRUReg<26, "r26.b2">;
def r26_b3 : PRUReg<26, "r26.b3">;
def r27_b0 : PRUReg<27, "r27.b0">;
def r27_b1 : PRUReg<27, "r27.b1">;
def r27_b2 : PRUReg<27, "r27.b2">;
def r27_b3 : PRUReg<27, "r27.b3">;
def r28_b0 : PRUReg<28, "r28.b0">;
def r28_b1 : PRUReg<28, "r28.b1">;
def r28_b2 : PRUReg<28, "r28.b2">;
def r28_b3 : PRUReg<28, "r28.b3">;
def r29_b0 : PRUReg<29, "r29.b0">;
def r29_b1 : PRUReg<29, "r29.b1">;
def r29_b2 : PRUReg<29, "r29.b2">;
def r29_b3 : PRUReg<29, "r29.b3">;
def r30_b0 : PRUReg<30, "r30.b0">;
def r30_b1 : PRUReg<30, "r30.b1">;
def r30_b2 : PRUReg<30, "r30.b2">;
def r30_b3 : PRUReg<30, "r30.b3">;
def r31_b0 : PRUReg<31, "r31.b0">;
def r31_b1 : PRUReg<31, "r31.b1">;
def r31_b2 : PRUReg<31, "r31.b2">;
def r31_b3 : PRUReg<31, "r31.b3">;
 
//// 16 bit subregs
let SubRegIndices = [sub_8_0, sub_8_8], CoveredBySubRegs = 1 in {

  def r0_w0 : PRUReg<0, "r0.w0", [r0_b0, r0_b1]>, DwarfRegNum<[]>;
  def r0_w2 : PRUReg<0, "r0.w2", [r0_b2, r0_b3]>, DwarfRegNum<[]>;
  def r0_w1 : PRUReg<0, "r0.w1", [r0_b1, r0_b2]>, DwarfRegNum<[]>;
  def r1_w0 : PRUReg<1, "r1.w0", [r1_b0, r1_b1]>, DwarfRegNum<[]>;
  def r1_w2 : PRUReg<1, "r1.w2", [r1_b2, r1_b3]>, DwarfRegNum<[]>;
  def r1_w1 : PRUReg<1, "r1.w1", [r1_b1, r1_b2]>, DwarfRegNum<[]>;
  def r2_w0 : PRUReg<2, "r2.w0", [r2_b0, r2_b1]>, DwarfRegNum<[]>;
  def r2_w2 : PRUReg<2, "r2.w2", [r2_b2, r2_b3]>, DwarfRegNum<[]>;
  def r2_w1 : PRUReg<2, "r2.w1", [r2_b1, r2_b2]>, DwarfRegNum<[]>;
  def r3_w0 : PRUReg<3, "r3.w0", [r3_b0, r3_b1]>, DwarfRegNum<[]>;
  def r3_w2 : PRUReg<3, "r3.w2", [r3_b2, r3_b3]>, DwarfRegNum<[]>;
  def r3_w1 : PRUReg<3, "r3.w1", [r3_b1, r3_b2]>, DwarfRegNum<[]>;
  def r4_w0 : PRUReg<4, "r4.w0", [r4_b0, r4_b1]>, DwarfRegNum<[]>;
  def r4_w2 : PRUReg<4, "r4.w2", [r4_b2, r4_b3]>, DwarfRegNum<[]>;
  def r4_w1 : PRUReg<4, "r4.w1", [r4_b1, r4_b2]>, DwarfRegNum<[]>;
  def r5_w0 : PRUReg<5, "r5.w0", [r5_b0, r5_b1]>, DwarfRegNum<[]>;
  def r5_w2 : PRUReg<5, "r5.w2", [r5_b2, r5_b3]>, DwarfRegNum<[]>;
  def r5_w1 : PRUReg<5, "r5.w1", [r5_b1, r5_b2]>, DwarfRegNum<[]>;
  def r6_w0 : PRUReg<6, "r6.w0", [r6_b0, r6_b1]>, DwarfRegNum<[]>;
  def r6_w2 : PRUReg<6, "r6.w2", [r6_b2, r6_b3]>, DwarfRegNum<[]>;
  def r6_w1 : PRUReg<6, "r6.w1", [r6_b1, r6_b2]>, DwarfRegNum<[]>;
  def r7_w0 : PRUReg<7, "r7.w0", [r7_b0, r7_b1]>, DwarfRegNum<[]>;
  def r7_w2 : PRUReg<7, "r7.w2", [r7_b2, r7_b3]>, DwarfRegNum<[]>;
  def r7_w1 : PRUReg<7, "r7.w1", [r7_b1, r7_b2]>, DwarfRegNum<[]>;
  def r8_w0 : PRUReg<8, "r8.w0", [r8_b0, r8_b1]>, DwarfRegNum<[]>;
  def r8_w2 : PRUReg<8, "r8.w2", [r8_b2, r8_b3]>, DwarfRegNum<[]>;
  def r8_w1 : PRUReg<8, "r8.w1", [r8_b1, r8_b2]>, DwarfRegNum<[]>;
  def r9_w0 : PRUReg<9, "r09.w0",[r9_b0, r9_b1]>, DwarfRegNum<[]>;
  def r9_w2 : PRUReg<9, "r9.w2", [r9_b2, r9_b3]>, DwarfRegNum<[]>;
  def r9_w1 : PRUReg<9, "r9.w1", [r9_b1, r9_b2]>, DwarfRegNum<[]>;
  def r10_w0 : PRUReg<10, "r10.w0", [r10_b0, r10_b1]>, DwarfRegNum<[]>;
  def r10_w2 : PRUReg<10, "r10.w2", [r10_b2, r10_b3]>, DwarfRegNum<[]>;
  def r10_w1 : PRUReg<10, "r10.w1", [r10_b1, r10_b2]>, DwarfRegNum<[]>;
  def r11_w0 : PRUReg<11, "r11.w0", [r11_b0, r11_b1]>, DwarfRegNum<[]>;
  def r11_w2 : PRUReg<11, "r11.w2", [r11_b2, r11_b3]>, DwarfRegNum<[]>;
  def r11_w1 : PRUReg<11, "r11.w1", [r11_b1, r11_b2]>, DwarfRegNum<[]>;
  def r12_w0 : PRUReg<12, "r12.w0", [r12_b0, r12_b1]>, DwarfRegNum<[]>;
  def r12_w2 : PRUReg<12, "r12.w2", [r12_b2, r12_b3]>, DwarfRegNum<[]>;
  def r12_w1 : PRUReg<12, "r12.w1", [r12_b1, r12_b2]>, DwarfRegNum<[]>;
  def r13_w0 : PRUReg<13, "r13.w0", [r13_b0, r13_b1]>, DwarfRegNum<[]>;
  def r13_w2 : PRUReg<13, "r13.w2", [r13_b2, r13_b3]>, DwarfRegNum<[]>;
  def r13_w1 : PRUReg<13, "r13.w1", [r13_b1, r13_b2]>, DwarfRegNum<[]>;
  def r14_w0 : PRUReg<14, "r14.w0", [r14_b0, r14_b1]>, DwarfRegNum<[]>;
  def r14_w2 : PRUReg<14, "r14.w2", [r14_b2, r14_b3]>, DwarfRegNum<[]>;
  def r14_w1 : PRUReg<14, "r14.w1", [r14_b1, r14_b2]>, DwarfRegNum<[]>;
  def r15_w0 : PRUReg<15, "r15.w0", [r15_b0, r15_b1]>, DwarfRegNum<[]>;
  def r15_w2 : PRUReg<15, "r15.w2", [r15_b2, r15_b3]>, DwarfRegNum<[]>;
  def r15_w1 : PRUReg<15, "r15.w1", [r15_b1, r15_b2]>, DwarfRegNum<[]>;
  def r16_w0 : PRUReg<16, "r16.w0", [r16_b0, r16_b1]>, DwarfRegNum<[]>;
  def r16_w2 : PRUReg<16, "r16.w2", [r16_b2, r16_b3]>, DwarfRegNum<[]>;
  def r16_w1 : PRUReg<16, "r16.w1", [r16_b1, r16_b2]>, DwarfRegNum<[]>;
  def r17_w0 : PRUReg<17, "r17.w0", [r17_b0, r17_b1]>, DwarfRegNum<[]>;
  def r17_w2 : PRUReg<17, "r17.w2", [r17_b2, r17_b3]>, DwarfRegNum<[]>;
  def r17_w1 : PRUReg<17, "r17.w1", [r17_b1, r17_b2]>, DwarfRegNum<[]>;
  def r18_w0 : PRUReg<18, "r18.w0", [r18_b0, r18_b1]>, DwarfRegNum<[]>;
  def r18_w2 : PRUReg<18, "r18.w2", [r18_b2, r18_b3]>, DwarfRegNum<[]>;
  def r18_w1 : PRUReg<18, "r18.w1", [r18_b1, r18_b2]>, DwarfRegNum<[]>;
  def r19_w0 : PRUReg<19, "r19.w0", [r19_b0, r19_b1]>, DwarfRegNum<[]>;
  def r19_w2 : PRUReg<19, "r19.w2", [r19_b2, r19_b3]>, DwarfRegNum<[]>;
  def r19_w1 : PRUReg<19, "r19.w1", [r19_b1, r19_b2]>, DwarfRegNum<[]>;
  def r20_w0 : PRUReg<20, "r20.w0", [r20_b0, r20_b1]>, DwarfRegNum<[]>;
  def r20_w2 : PRUReg<20, "r20.w2", [r20_b2, r20_b3]>, DwarfRegNum<[]>;
  def r20_w1 : PRUReg<20, "r20.w1", [r20_b1, r20_b2]>, DwarfRegNum<[]>;
  def r21_w0 : PRUReg<21, "r21.w0", [r21_b0, r21_b1]>, DwarfRegNum<[]>;
  def r21_w2 : PRUReg<21, "r21.w2", [r21_b2, r21_b3]>, DwarfRegNum<[]>;
  def r21_w1 : PRUReg<21, "r21.w1", [r21_b1, r21_b2]>, DwarfRegNum<[]>;
  def r22_w0 : PRUReg<22, "r22.w0", [r22_b0, r22_b1]>, DwarfRegNum<[]>;
  def r22_w2 : PRUReg<22, "r22.w2", [r22_b2, r22_b3]>, DwarfRegNum<[]>;
  def r22_w1 : PRUReg<22, "r22.w1", [r22_b1, r22_b2]>, DwarfRegNum<[]>;
  def r23_w0 : PRUReg<23, "r23.w0", [r23_b0, r23_b1]>, DwarfRegNum<[]>;
  def r23_w2 : PRUReg<23, "r23.w2", [r23_b2, r23_b3]>, DwarfRegNum<[]>;
  def r23_w1 : PRUReg<23, "r23.w1", [r23_b1, r23_b2]>, DwarfRegNum<[]>;
  def r24_w0 : PRUReg<24, "r24.w0", [r24_b0, r24_b1]>, DwarfRegNum<[]>;
  def r24_w2 : PRUReg<24, "r24.w2", [r24_b2, r24_b3]>, DwarfRegNum<[]>;
  def r24_w1 : PRUReg<24, "r24.w1", [r24_b1, r24_b2]>, DwarfRegNum<[]>;
  def r25_w0 : PRUReg<25, "r25.w0", [r25_b0, r25_b1]>, DwarfRegNum<[]>;
  def r25_w2 : PRUReg<25, "r25.w2", [r25_b2, r25_b3]>, DwarfRegNum<[]>;
  def r25_w1 : PRUReg<25, "r25.w1", [r25_b1, r25_b2]>, DwarfRegNum<[]>;
  def r26_w0 : PRUReg<26, "r26.w0", [r26_b0, r26_b1]>, DwarfRegNum<[]>;
  def r26_w2 : PRUReg<26, "r26.w2", [r26_b2, r26_b3]>, DwarfRegNum<[]>;
  def r26_w1 : PRUReg<26, "r26.w1", [r26_b1, r26_b2]>, DwarfRegNum<[]>;
  def r27_w0 : PRUReg<27, "r27.w0", [r27_b0, r27_b1]>, DwarfRegNum<[]>;
  def r27_w2 : PRUReg<27, "r27.w2", [r27_b2, r27_b3]>, DwarfRegNum<[]>;
  def r27_w1 : PRUReg<27, "r27.w1", [r27_b1, r27_b2]>, DwarfRegNum<[]>;
  def r28_w0 : PRUReg<28, "r28.w0", [r28_b0, r28_b1]>, DwarfRegNum<[]>;
  def r28_w2 : PRUReg<28, "r28.w2", [r28_b2, r28_b3]>, DwarfRegNum<[]>;
  def r28_w1 : PRUReg<28, "r28.w1", [r28_b1, r28_b2]>, DwarfRegNum<[]>;
  def r29_w0 : PRUReg<29, "r29.w0", [r29_b0, r29_b1]>, DwarfRegNum<[]>;
  def r29_w2 : PRUReg<29, "r29.w2", [r29_b2, r29_b3]>, DwarfRegNum<[]>;
  def r29_w1 : PRUReg<29, "r29.w1", [r29_b1, r29_b2]>, DwarfRegNum<[]>;
  def r30_w0 : PRUReg<30, "r30.w0", [r30_b0, r30_b1]>, DwarfRegNum<[]>;
  def r30_w2 : PRUReg<30, "r30.w2", [r30_b2, r30_b3]>, DwarfRegNum<[]>;
  def r30_w1 : PRUReg<30, "r30.w1", [r30_b1, r30_b2]>, DwarfRegNum<[]>;
  def r31_w0 : PRUReg<31, "r31.w0", [r31_b0, r31_b1]>, DwarfRegNum<[]>;
  def r31_w2 : PRUReg<31, "r31.w2", [r31_b2, r31_b3]>, DwarfRegNum<[]>;
  def r31_w1 : PRUReg<31, "r31.w1", [r31_b1, r31_b2]>, DwarfRegNum<[]>;
}
 
//// 32 bit main regs
let SubRegIndices = [sub_16_0, sub_16_16], CoveredBySubRegs = 1 in {
  def r0 : PRUReg<0, "r0", [r0_w0, r0_w2]>, DwarfRegNum<[0]>;
  def r1 : PRUReg<1, "r1", [r1_w0, r1_w2]>, DwarfRegNum<[1]>;
  def r2 : PRUReg<2, "r2", [r2_w0, r2_w2]>, DwarfRegNum<[2]>;
  def r3 : PRUReg<3, "r3", [r2_w0, r3_w2]>, DwarfRegNum<[3]>;
  def r4 : PRUReg<4, "r4", [r4_w0, r4_w2]>, DwarfRegNum<[4]>;
  def r5 : PRUReg<5, "r5", [r5_w0, r5_w2]>, DwarfRegNum<[5]>;
  def r6 : PRUReg<6, "r6", [r6_w0, r6_w2]>, DwarfRegNum<[6]>;
  def r7 : PRUReg<7, "r7", [r7_w0, r7_w2]>, DwarfRegNum<[7]>;
  def r8 : PRUReg<8, "r8", [r8_w0, r8_w2]>, DwarfRegNum<[8]>;
  def r9 : PRUReg<9, "r9", [r9_w0, r9_w2]>, DwarfRegNum<[9]>;
  def r10 : PRUReg<10, "r10", [r10_w0, r10_w2]>, DwarfRegNum<[10]>;
  def r11 : PRUReg<11, "r11", [r11_w0, r11_w2]>, DwarfRegNum<[11]>;
  def r12 : PRUReg<12, "r12", [r12_w0, r12_w2]>, DwarfRegNum<[12]>;
  def r13 : PRUReg<13, "r13", [r13_w0, r13_w2]>, DwarfRegNum<[13]>;
  def r14 : PRUReg<14, "r14", [r14_w0, r14_w2]>, DwarfRegNum<[14]>;
  def r15 : PRUReg<15, "r15", [r15_w0, r15_w2]>, DwarfRegNum<[15]>;
  def r16 : PRUReg<16, "r16", [r16_w0, r16_w2]>, DwarfRegNum<[16]>;
  def r17 : PRUReg<17, "r17", [r17_w0, r17_w2]>, DwarfRegNum<[17]>;
  def r18 : PRUReg<18, "r18", [r18_w0, r18_w2]>, DwarfRegNum<[18]>;
  def r19 : PRUReg<19, "r19", [r19_w0, r19_w2]>, DwarfRegNum<[19]>;
  def r20 : PRUReg<20, "r20", [r20_w0, r20_w2]>, DwarfRegNum<[20]>;
  def r21 : PRUReg<21, "r21", [r21_w0, r21_w2]>, DwarfRegNum<[21]>;
  def r22 : PRUReg<22, "r22", [r22_w0, r22_w2]>, DwarfRegNum<[22]>;
  def r23 : PRUReg<23, "r23", [r23_w0, r23_w2]>, DwarfRegNum<[23]>;
  def r24 : PRUReg<24, "r24", [r24_w0, r24_w2]>, DwarfRegNum<[24]>;
  def r25 : PRUReg<25, "r25", [r25_w0, r25_w2]>, DwarfRegNum<[25]>;
  def r26 : PRUReg<26, "r26", [r26_w0, r26_w2]>, DwarfRegNum<[26]>;
  def r27 : PRUReg<27, "r27", [r27_w0, r27_w2]>, DwarfRegNum<[27]>;
  def r28 : PRUReg<28, "r28", [r28_w0, r28_w2]>, DwarfRegNum<[28]>;
  def r29 : PRUReg<29, "r29", [r29_w0, r29_w2]>, DwarfRegNum<[29]>;
  def r30 : PRUReg<30, "r30", [r30_w0, r30_w2]>, DwarfRegNum<[30]>;
  def r31 : PRUReg<31, "r31", [r31_w0, r31_w2]>, DwarfRegNum<[31]>;

}
 
let SubRegIndices = [sub_8_8], CoveredBySubRegs = 1 in {
  def b0: PRUReg<32, "b0", [r0_b0]>, DwarfRegNum<[]>;
  def b1: PRUReg<32, "b1", [r0_b1]>, DwarfRegNum<[]>;
  def b2: PRUReg<32, "b2", [r0_b2]>, DwarfRegNum<[]>;
  def b3: PRUReg<32, "b3", [r0_b3]>, DwarfRegNum<[]>;
}

//===----------------------------------------------------------------------===//
//  Register Classes
//===----------------------------------------------------------------------===//

def breg : RegisterClass<"PRU", [i8], 8, (sequence "b%u", 0, 3)>;

def reg8 : RegisterClass<"PRU", [i8], 8, (add
    (sequence "r%u_b0", 0, 31),
    (sequence "r%u_b1", 0, 31),
    (sequence "r%u_b2", 0, 31),
    (sequence "r%u_b3", 0, 31)
)>;

def reg16 : RegisterClass<"PRU", [i16], 8, (add
    (sequence "r%u_w0", 0, 31),
    (sequence "r%u_w1", 0, 31),
    (sequence "r%u_w2", 0, 31)
)>;

def reg32 : RegisterClass<"PRU", [i32], 8, (sequence "r%u", 0, 31)>;

def GPRegs : RegisterClass<"PRU", [i32], 32, (add
	(sequence "r%u", 4, 30), r2, r3
)>;