add_llvm_component_library(LLVMPRUDesc
    PRUInstPrinter.cpp
    PRUMCAsmInfo.cpp
    PRUMCCodeEmitter.cpp
    PRUMCTargetDesc.cpp
    PRUTargetStreamer.cpp

    LINK_COMPONENTS
    MC
    PRUInfo
    Support
    TargetParser

    ADD_TO_COMPONENT
    PRU
)

add_dependencies(LLVMPRUDesc PRUCommonTableGen)