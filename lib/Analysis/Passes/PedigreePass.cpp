//
//
//

#include "Utils.hpp"

#include "Analysis/Passes/PedigreePass.hpp"

#include "llvm/IR/Type.h"
// using llvm::Type

#include "llvm/IR/Instruction.h"
// using llvm::Instruction

#include "llvm/IR/Function.h"
// using llvm::Function

#include "llvm/Support/Casting.h"
// using llvm::dyn_cast

#include "llvm/Pass.h"
// using llvm::RegisterPass

#include "llvm/IR/LegacyPassManager.h"
// using llvm::PassManagerBase

#include "llvm/Transforms/IPO/PassManagerBuilder.h"
// using llvm::PassManagerBuilder
// using llvm::RegisterStandardPasses

#include "llvm/Support/CommandLine.h"
// using llvm::cl::opt
// using llvm::cl::desc
// using llvm::cl::location
// using llvm::cl::cat
// using llvm::cl::OptionCategory

#include "llvm/Support/raw_ostream.h"
// using llvm::raw_ostream

#include "llvm/Support/Debug.h"
// using DEBUG macro
// using llvm::dbgs

#define DEBUG_TYPE "pedigree"

// plugin registration for opt

char pedigree::PedigreePass::ID = 0;
static llvm::RegisterPass<pedigree::PedigreePass>
    X("pedigree", PRJ_CMDLINE_DESC("pedigree pass"), false, false);

// plugin registration for clang

// the solution was at the bottom of the header file
// 'llvm/Transforms/IPO/PassManagerBuilder.h'
// create a static free-floating callback that uses the legacy pass manager to
// add an instance of this pass and a static instance of the
// RegisterStandardPasses class

static void registerPedigreePass(const llvm::PassManagerBuilder &Builder,
                                 llvm::legacy::PassManagerBase &PM) {
  PM.add(new pedigree::PedigreePass());

  return;
}

static llvm::RegisterStandardPasses
    RegisterPedigreePass(llvm::PassManagerBuilder::EP_EarlyAsPossible,
                         registerPedigreePass);

//

static llvm::cl::OptionCategory
    PedigreePassCategory("Pedigree Pass", "Options for Pedigree pass");

#if PEDIGREE_DEBUG
static llvm::cl::opt<bool, true>
    Debug("pedigree-debug", llvm::cl::desc("debug pedigree pass"),
          llvm::cl::location(pedigree::utility::passDebugFlag),
          llvm::cl::cat(PedigreePassCategory));

static llvm::cl::opt<LogLevel, true> DebugLevel(
    "pedigree-debug-level", llvm::cl::desc("debug level for pedigree pass"),
    llvm::cl::location(pedigree::utility::passLogLevel),
    llvm::cl::values(
        clEnumValN(LogLevel::info, "info", "informational messages"),
        clEnumValN(LogLevel::notice, "notice", "significant conditions"),
        clEnumValN(LogLevel::warning, "warning", "warning conditions"),
        clEnumValN(LogLevel::error, "error", "error conditions"),
        clEnumValN(LogLevel::debug, "debug", "debug messages"), nullptr),
    llvm::cl::cat(PedigreePassCategory));
#endif // PEDIGREE_DEBUG

//

namespace pedigree {

bool PedigreePass::runOnFunction(llvm::Function &CurFunc) { return false; }

} // namespace pedigree end