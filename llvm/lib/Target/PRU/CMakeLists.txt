add_llvm_component_group(PRU)
set(LLVM_TARGET_DEFINITIONS PRU.td)

tablegen(LLVM PRUGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM PRUGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM PRUGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM PRUGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM PRUGenCallingConv.inc -gen-callingconv)
tablegen(LLVM PRUGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM PRUGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM PRUGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM PRUGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(PRUCommonTableGen)

add_llvm_target(PRUCodeGen
    PRUAsmPrinter.cpp
    PRUFrameLowering.cpp
    PRUInstrInfo.cpp
    PRUISelDagToDag.cpp
    PRUISelLowering.cpp
    PRURegisterInfo.cpp
    PRUSubtarget.cpp
    PRUTargetMachine.cpp
    PRUTargetObjectFile.cpp

    DEPENDS
    intrinsics_gen

    LINK_COMPONENTS
    AsmPrinter
    CodeGen
    Core
    MC
    PRUDesc
    PRUInfo
    SelectionDAG
    Support
    Target

    ADD_TO_COMPONENT
    PRU
)

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

