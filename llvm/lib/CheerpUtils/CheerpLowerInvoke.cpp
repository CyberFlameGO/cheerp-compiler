//===-- CheerpLowerInvoke.cpp - Cheerp optimization pass ------------------------===//
//
//                     Cheerp: The C++ compiler for the Web
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
// Copyright 2021 Leaning Technologies
//
//===----------------------------------------------------------------------===//

#include "llvm/Cheerp/CFGPasses.h"
#include "llvm/Cheerp/CommandLine.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/InitializePasses.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

#define DEBUG_TYPE "cheerplowerinvoke"

STATISTIC(NumInvokes, "Number of invokes replaced");

namespace {
  class CheerpLowerInvoke : public FunctionPass {
  public:
    static char ID; // Pass identification, replacement for typeid
    explicit CheerpLowerInvoke() : FunctionPass(ID) {
    }
    bool runOnFunction(Function &F) override;
  };
}

char CheerpLowerInvoke::ID = 0;
INITIALIZE_PASS(CheerpLowerInvoke, "CheerpLowerInvoke",
                "Lower invoke to call",
                false, false)

bool CheerpLowerInvoke::runOnFunction(Function &F) {
  if(KeepInvokes)
    return false;
  bool Changed = false;
  F.setPersonalityFn(nullptr);
  for (BasicBlock &BB : F)
    if (InvokeInst *II = dyn_cast<InvokeInst>(BB.getTerminator())) {
      SmallVector<Value *, 16> CallArgs(II->arg_begin(), II->arg_end());
      SmallVector<OperandBundleDef, 1> OpBundles;
      II->getOperandBundlesAsDefs(OpBundles);
      // Insert a normal call instruction...
      CallInst *NewCall =
          CallInst::Create(II->getFunctionType(), II->getCalledOperand(),
                           CallArgs, OpBundles, "", II);
      NewCall->takeName(II);
      NewCall->setCallingConv(II->getCallingConv());
      NewCall->setAttributes(II->getAttributes());
      NewCall->setDebugLoc(II->getDebugLoc());
      II->replaceAllUsesWith(NewCall);

      // Insert an unconditional branch to the normal destination.
      BranchInst::Create(II->getNormalDest(), II);

      // Remove any PHI node entries from the exception destination.
      II->getUnwindDest()->removePredecessor(&BB);

      // Remove the invoke instruction now.
      BB.getInstList().erase(II);

      ++NumInvokes;
      Changed = true;
    }
  if(Changed)
    llvm::EliminateUnreachableBlocks(F);
  return Changed;
}

namespace llvm {
// Public Interface To the CheerpLowerInvoke pass.
FunctionPass *createCheerpLowerInvokePass() { return new CheerpLowerInvoke(); }

}
