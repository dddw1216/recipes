// Copyright (c) 2024, dddw1216. All rights reserved.

// Command:
// sudo apt-get install llvm
// sudo apt-get install llvm-dev
// g++ normal.cc -o normal -O2

// Output:
// ./normal
// Sum: 2.79997e+10
// Normal function execution time: 0.00130494 s
// ./jit
// Sum: 2.79997e+10
// JIT Execution time: 0.00106156 s

#include <chrono>
#include <iostream>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Utils.h>

using namespace llvm;

typedef double (*FuncType)(double);

int main() {
  // Initialize the target
  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  // Create a context and a module
  LLVMContext context;
  auto module = std::make_unique<Module>("my module", context);
  IRBuilder<> builder(context);

  // Create function signature double(double)
  FunctionType *funcType = FunctionType::get(Type::getDoubleTy(context), {Type::getDoubleTy(context)}, false);
  Function *func = Function::Create(funcType, Function::ExternalLinkage, "func", module.get());

  // Build the function body
  BasicBlock *entry = BasicBlock::Create(context, "entry", func);
  builder.SetInsertPoint(entry);
  Argument *argX = func->getArg(0);
  Value *result = argX;

  // Create a simple arithmetic series
  for (int i = 1; i <= 10; ++i) {
    Value *coeff = ConstantFP::get(context, APFloat(static_cast<double>(i)));
    result = builder.CreateFAdd(result, builder.CreateFMul(coeff, argX), "addtmp");
  }
  builder.CreateRet(result);

  // Verify the correctness of the IR
  std::string error;
  raw_string_ostream error_stream(error);
  if (verifyFunction(*func, &error_stream)) {
    std::cerr << "Function verification failed: " << error_stream.str() << std::endl;
    return 1;
  }
  if (verifyModule(*module, &error_stream)) {
    std::cerr << "Module verification failed: " << error_stream.str() << std::endl;
    return 1;
  }

  // Use a raw pointer to the module
  auto rawModule = module.get();

  // Create the JIT execution engine
  std::string errStr;
  ExecutionEngine *execEngine = EngineBuilder(std::move(module))
                                    .setErrorStr(&errStr)
                                    .setMCJITMemoryManager(std::make_unique<SectionMemoryManager>())
                                    .create();
  if (!execEngine) {
    std::cerr << "Failed to create ExecutionEngine: " << errStr << std::endl;
    return 1;
  }

  // Create and apply optimizations via a pass manager
  llvm::legacy::FunctionPassManager fpm(rawModule);
  fpm.add(createPromoteMemoryToRegisterPass());
  fpm.add(createInstructionCombiningPass());
  fpm.add(createReassociatePass());
  fpm.add(createGVNPass());
  fpm.add(createCFGSimplificationPass());
  fpm.doInitialization();
  fpm.run(*func);
  fpm.doFinalization();

  // Get the function pointer and execute it
  auto fnPointer = reinterpret_cast<FuncType>(execEngine->getFunctionAddress("func"));
  if (!fnPointer) {
    std::cerr << "Could not get function pointer." << std::endl;
    return 1;
  }

  double sum = 0;
  const int N = 100000;
  auto exec_start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < N; ++i) {
    sum += fnPointer(i * 0.1);
  }

  auto exec_end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> exec_diff = exec_end - exec_start;

  std::cout << "Sum: " << sum << std::endl;
  std::cout << "JIT Execution time: " << exec_diff.count() << " s" << std::endl;

  return 0;
}