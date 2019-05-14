//===-- Pedigree.cpp - Pedigree Infrastructure ----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the common initialization infrastructure for the Pedigree
// library.
//
//===----------------------------------------------------------------------===//

#include "llvm/InitializePasses.h"
#include "llvm/PassRegistry.h"

using namespace llvm;

/// initializePedigree - Initialize all passes in the Pedigree library.
void llvm::initializePedigree(PassRegistry &Registry) {
  initializePDGraphWrapperPassPass(Registry);
  initializeCDGraphWrapperPassPass(Registry);
  initializeDDGraphWrapperPassPass(Registry);
  initializeMDGraphWrapperPassPass(Registry);
}
