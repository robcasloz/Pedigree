//===- Pedigree.h - Pedigree ------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This header file defines prototypes for accessor functions that expose passes
// in the Pedigree library.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_PEDIGREE_H
#define LLVM_PEDIGREE_H

namespace llvm {

class FunctionPass;

FunctionPass *createPDGraphWrapperPass();
FunctionPass *createCDGraphWrapperPass();
FunctionPass *createDDGraphWrapperPass();
FunctionPass *createMDGraphWrapperPass();

} // End llvm namespace

#endif
