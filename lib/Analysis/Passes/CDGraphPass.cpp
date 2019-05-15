//
//
//

#include "llvm/Transforms/Utils.h"

#include "Pedigree/Config.hpp"

#include "Pedigree/Debug.hpp"

#include "Pedigree/Util.hpp"

#include "Pedigree/Analysis/Passes/CDGraphPass.hpp"

#include "Pedigree/Analysis/Creational/CDGraphBuilder.hpp"

#include "Pedigree/Support/GraphConverter.hpp"

#include "Pedigree/Support/Utils/UnitConverters.hpp"

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
// using llvm::cl::OptionCategory

#include "llvm/Support/raw_ostream.h"
// using llvm::raw_ostream

#include "llvm/Support/Debug.h"
// using LLVM_DEBUG macro
// using llvm::dbgs

#include <utility>
// using std::move

#define DEBUG_TYPE PEDIGREE_CDG_PASS_NAME

extern llvm::cl::opt<bool> PedigreeGraphConnectRoot;

namespace llvm {
class Function;
} // namespace llvm

// plugin registration for opt

char pedigree::CDGraphWrapperPass::ID = 0;
char &llvm::CDGraphID = pedigree::CDGraphWrapperPass::ID;

using namespace llvm;
using namespace pedigree;
INITIALIZE_PASS_BEGIN(CDGraphWrapperPass, PEDIGREE_CDG_PASS_NAME, PRJ_CMDLINE_DESC("pedigree cdg pass"), false, true)
INITIALIZE_PASS_END(CDGraphWrapperPass, PEDIGREE_CDG_PASS_NAME, PRJ_CMDLINE_DESC("pedigree cdg pass"), false, true)

namespace llvm {
  FunctionPass *llvm::createCDGraphWrapperPass() {
    return new pedigree::CDGraphWrapperPass();
  }
}

static llvm::cl::OptionCategory
    PedigreeCDGraphPassCategory("Pedigree CDGraph Pass",
                                "Options for Pedigree CDGraph pass");

static llvm::cl::opt<bool> PedigreeCDGraphConvertToInstruction(
    "pedigree-cdg-convert-instruction",
    llvm::cl::desc("adapt cdg to block terminator instructions"),
    llvm::cl::init(false), llvm::cl::cat(PedigreeCDGraphPassCategory));

//

llvm::AnalysisKey pedigree::CDGraphAnalysis::Key;

namespace pedigree {

// new passmanager pass

CDGraphAnalysis::Result
CDGraphAnalysis::run(llvm::Function &F, llvm::FunctionAnalysisManager &FAM) {
  CDGraphBuilder builder{};
  auto graph = builder.setUnit(F).build();

  if (PedigreeGraphConnectRoot) {
    graph->connectRootNode();
  }

  if (PedigreeCDGraphConvertToInstruction) {
    // TODO do nothing for now
    // instGraph = std::make_unique<InstCDGraph>();
    // Convert(*graph, *instGraph, BlockToTerminatorUnitConverter{},
    // BlockToInstructionsUnitConverter{});
  }

  return std::move(graph);
}

// legacy passmanager pass

CDGraphWrapperPass::CDGraphWrapperPass() : llvm::FunctionPass(ID) {
  initializeCDGraphWrapperPassPass(*PassRegistry::getPassRegistry());
}

void CDGraphWrapperPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const {
  AU.setPreservesAll();
}

bool CDGraphWrapperPass::runOnFunction(llvm::Function &F) {
  CDGraphBuilder builder{};
  Graph = builder.setUnit(F).build();

  if (PedigreeGraphConnectRoot) {
    Graph->connectRootNode();
  }

  if (PedigreeCDGraphConvertToInstruction) {
    InstGraph = std::make_unique<InstCDGraph>();
    Convert(*Graph, *InstGraph, BlockToTerminatorUnitConverter{},
            BlockToInstructionsUnitConverter{});
  }

  return false;
}

} // namespace pedigree
