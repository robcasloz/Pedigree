# Pedigree
add_llvm_library(LLVMPedigree
  Pedigree.cpp
  Analysis/Hazards.cpp
  Analysis/Graphs/DependenceGraphs.cpp
  Analysis/Passes/MDGraphPass.cpp
  Analysis/Passes/CDGraphPass.cpp
  Analysis/Passes/Printers/Graphviz/GraphPrinterPassCommandLineOptions.cpp
  Analysis/Passes/Printers/Graphviz/PDGraphPrinterPass.cpp
  Analysis/Passes/Printers/Graphviz/MDGraphPrinterPass.cpp
  Analysis/Passes/Printers/Graphviz/DDGraphPrinterPass.cpp
  Analysis/Passes/Printers/Graphviz/CDGraphPrinterPass.cpp
  Analysis/Passes/Printers/Graphviz/InstCDGraphPrinterPass.cpp
  Analysis/Passes/PDGraphPass.cpp
  Analysis/Passes/DDGraphPass.cpp
  Analysis/Passes/GraphPassCommandLineOptions.cpp
  Support/FileSystem.cpp
  RegisterPasses.cpp
  Debug.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/Analysis

  DEPENDS
  intrinsics_gen
  )
