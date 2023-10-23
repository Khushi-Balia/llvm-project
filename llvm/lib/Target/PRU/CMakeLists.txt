set(LLVM_TARGET_DEFINITIONS PRU.td)

tablegen(LLVM PRUGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM PRUGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM PRUGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM PRUGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM PRUGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM PRUGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM PRUGenCallingConv.inc -gen-callingconv)
tablegen(LLVM PRUGenDAGISel.inc -gen-dag-isel)

add_public_tablegen_target(PRUCommonTableGen)

add_llvm_target(PRUCodeGen
  PRUAsmPrinter.cpp
  PRUFrameLowering.cpp
  PRUInstrInfo.cpp
  PRUISelDAGToDAG.cpp
  PRUISelLowering.cpp
  PRUMCInstLower.cpp
  PRURegisterInfo.cpp
  PRUSubtarget.cpp
  PRUTargetMachine.cpp
  PRUMachineFunctionInfo.cpp
  )

add_subdirectory(InstPrinter)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
