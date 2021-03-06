//
//
//

#include "Pedigree/Config.hpp"

#include "Pedigree/Debug.hpp"

#include "Pedigree/Util.hpp"

#include "Pedigree/Analysis/Passes/MDGraphPass.hpp"

#include "Pedigree/Support/Traits/LLVMAnalysisGraphTraits.hpp"

#include "Pedigree/Support/FileSystem.hpp"

#include "llvm/Pass.h"
// using llvm::RegisterPass

#include "llvm/IR/LegacyPassManager.h"
// using llvm::PassManagerBase

#include "llvm/Transforms/IPO/PassManagerBuilder.h"
// using llvm::PassManagerBuilder
// using llvm::RegisterStandardPasses

#include "llvm/Analysis/DOTGraphTraitsPass.h"
// using llvm::DOTGraphTraitsPrinter

#include "llvm/Support/CommandLine.h"
// using llvm::cl::list

#include "llvm/Support/raw_ostream.h"
// using llvm::raw_string_ostream

#include "llvm/Support/ErrorHandling.h"
// using llvm::report_fatal_error

#include "llvm/Support/Debug.h"
// using LLVM_DEBUG macro
// using llvm::dbgs

#include <string>
// using std::string

#include <algorithm>
// std::find

#include <system_error>
// using std::error_code

#define DEBUG_TYPE "pedigree-mdg-printer"

extern llvm::cl::list<std::string> PedigreeGraphDOTFunctionWhitelist;
extern llvm::cl::opt<std::string> PedigreeReportsDir;

namespace pedigree {

struct MDGraphPrinterWrapperPass
    : public llvm::DOTGraphTraitsPrinter<
          MDGraphWrapperPass, false, MDGraph *,
          LLVMAnalysisStandardDependenceGraphPassTraitsHelperBase<
              MDGraphWrapperPass, MDGraph>> {
  using Base = llvm::DOTGraphTraitsPrinter<
      MDGraphWrapperPass, false, MDGraph *,
      LLVMAnalysisStandardDependenceGraphPassTraitsHelperBase<
          MDGraphWrapperPass, MDGraph>>;

  static char ID;

  MDGraphPrinterWrapperPass() : Base(PedigreeReportsDir + "/" + "mdg", ID) {
    auto dirOrErr = CreateDirectory(PedigreeReportsDir);
    if (std::error_code ec = dirOrErr.getError()) {
      llvm::errs() << "Error: " << ec.message() << '\n';
      llvm::report_fatal_error("Failed to create reports directory" +
                               PedigreeReportsDir);
    }

    PedigreeReportsDir = dirOrErr.get();
  }

  bool runOnFunction(llvm::Function &CurFunction) override {
    auto found = std::find(std::begin(PedigreeGraphDOTFunctionWhitelist),
                           std::end(PedigreeGraphDOTFunctionWhitelist),
                           CurFunction.getName().str());
    auto hasChanged = false;

    if (PedigreeGraphDOTFunctionWhitelist.empty() ||
        std::end(PedigreeGraphDOTFunctionWhitelist) != found) {
      hasChanged |= Base::runOnFunction(CurFunction);
    }

    return hasChanged;
  }
};

//

struct MDGraphSimplePrinterWrapperPass
    : public llvm::DOTGraphTraitsPrinter<
          MDGraphWrapperPass, true, MDGraph *,
          LLVMAnalysisStandardDependenceGraphPassTraitsHelperBase<
              MDGraphWrapperPass, MDGraph>> {
  using Base = llvm::DOTGraphTraitsPrinter<
      MDGraphWrapperPass, true, MDGraph *,
      LLVMAnalysisStandardDependenceGraphPassTraitsHelperBase<
          MDGraphWrapperPass, MDGraph>>;

  static char ID;

  MDGraphSimplePrinterWrapperPass()
      : Base(PedigreeReportsDir + "/" + "mdg", ID) {
    auto dirOrErr = CreateDirectory(PedigreeReportsDir);
    if (std::error_code ec = dirOrErr.getError()) {
      llvm::errs() << "Error: " << ec.message() << '\n';
      llvm::report_fatal_error("Failed to create reports directory" +
                               PedigreeReportsDir);
    }

    PedigreeReportsDir = dirOrErr.get();
  }

  bool runOnFunction(llvm::Function &CurFunction) override {
    auto found = std::find(std::begin(PedigreeGraphDOTFunctionWhitelist),
                           std::end(PedigreeGraphDOTFunctionWhitelist),
                           CurFunction.getName().str());
    auto hasChanged = false;

    if (PedigreeGraphDOTFunctionWhitelist.empty() ||
        std::end(PedigreeGraphDOTFunctionWhitelist) != found) {
      hasChanged = Base::runOnFunction(CurFunction);
    }

    return hasChanged;
  }
};

} // namespace pedigree

char pedigree::MDGraphPrinterWrapperPass::ID = 0;
static llvm::RegisterPass<pedigree::MDGraphPrinterWrapperPass>
    X("pedigree-mdg-dot", PRJ_CMDLINE_DESC("pedigree mdg DOT pass"), false,
      false);

//

char pedigree::MDGraphSimplePrinterWrapperPass::ID = 0;
static llvm::RegisterPass<pedigree::MDGraphSimplePrinterWrapperPass>
    Y("pedigree-mdg-simple-dot",
      PRJ_CMDLINE_DESC("pedigree simple mdg DOT pass"), false, false);
