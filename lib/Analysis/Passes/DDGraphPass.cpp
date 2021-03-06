//
//
//

#include "llvm/Transforms/Utils.h"

#include "Pedigree/Debug.hpp"

#include "Pedigree/Util.hpp"

#include "Pedigree/Analysis/Passes/DDGraphPass.hpp"

#include "Pedigree/Analysis/Creational/DDGraphBuilder.hpp"

#include "llvm/Config/llvm-config.h"
// version macros

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
// using llvm::cl::init
// using llvm::cl::Hidden
// using llvm::cl::OptionCategory

#include "llvm/Support/raw_ostream.h"
// using llvm::raw_ostream

#include "llvm/Support/Debug.h"
// using DEBUG macro
// using llvm::dbgs

#include <utility>
// using std::move

#define DEBUG_TYPE PEDIGREE_DDG_PASS_NAME

extern llvm::cl::opt<bool> PedigreeGraphConnectRoot;

namespace llvm {
class Function;
} // namespace llvm

// plugin registration for opt

char pedigree::DDGraphWrapperPass::ID = 0;
char &llvm::DDGraphID = pedigree::DDGraphWrapperPass::ID;

using namespace llvm;
using namespace pedigree;
INITIALIZE_PASS_BEGIN(DDGraphWrapperPass, PEDIGREE_DDG_PASS_NAME, PRJ_CMDLINE_DESC("pedigree ddg pass"), false, true)
INITIALIZE_PASS_END(DDGraphWrapperPass, PEDIGREE_DDG_PASS_NAME, PRJ_CMDLINE_DESC("pedigree ddg pass"), false, true)

namespace llvm {
  FunctionPass *llvm::createDDGraphWrapperPass() {
    return new pedigree::DDGraphWrapperPass();
  }
}

static llvm::cl::OptionCategory
    PedigreeDDGraphWrapperPassCategory("Pedigree DDGraph Pass",
                                       "Options for Pedigree DDGraph pass");

static llvm::cl::opt<bool> IgnoreConstantPHINodes(
    "pedigree-ddg-ignore-constant-phi", llvm::cl::init(true), llvm::cl::Hidden,
    llvm::cl::desc("ignore PHI nodes with constant values as data dependences"),
    llvm::cl::cat(PedigreeDDGraphWrapperPassCategory));

//

llvm::AnalysisKey pedigree::DDGraphAnalysis::Key;

namespace pedigree {

// new passmanager pass

DDGraphAnalysis::Result
DDGraphAnalysis::run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
  DDGraphBuilder builder{};
  auto graph =
      builder.setUnit(F).ignoreConstantPHINodes(IgnoreConstantPHINodes).build();

  if (PedigreeGraphConnectRoot) {
    graph->connectRootNode();
  }

  return std::move(graph);
}

// legacy passmanager pass

DDGraphWrapperPass::DDGraphWrapperPass() : llvm::FunctionPass(ID) {
  initializeDDGraphWrapperPassPass(*PassRegistry::getPassRegistry());
}

void DDGraphWrapperPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

bool DDGraphWrapperPass::runOnFunction(llvm::Function &F) {
  DDGraphBuilder builder{};
  Graph =
      builder.setUnit(F).ignoreConstantPHINodes(IgnoreConstantPHINodes).build();

  if (PedigreeGraphConnectRoot) {
    Graph->connectRootNode();
  }

  return false;
}

} // namespace pedigree
