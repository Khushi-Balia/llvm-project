//===--- Context.h - Context for the constexpr VM ---------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Defines the constexpr execution context.
//
// The execution context manages cached bytecode and the global context.
// It invokes the compiler and interpreter, propagating errors.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_AST_INTERP_CONTEXT_H
#define LLVM_CLANG_AST_INTERP_CONTEXT_H

#include "InterpStack.h"
#include "clang/AST/APValue.h"

namespace clang {
class ASTContext;
class LangOptions;
class FunctionDecl;
class VarDecl;

namespace interp {
class Function;
class Program;
class State;
enum PrimType : unsigned;

/// Holds all information required to evaluate constexpr code in a module.
class Context final {
public:
  /// Initialises the constexpr VM.
  Context(ASTContext &Ctx);

  /// Cleans up the constexpr VM.
  ~Context();

  /// Checks if a function is a potential constant expression.
  bool isPotentialConstantExpr(State &Parent, const FunctionDecl *FnDecl);

  /// Evaluates a toplevel expression as an rvalue.
  bool evaluateAsRValue(State &Parent, const Expr *E, APValue &Result);

  /// Evaluates a toplevel initializer.
  bool evaluateAsInitializer(State &Parent, const VarDecl *VD, APValue &Result);

  /// Returns the AST context.
  ASTContext &getASTContext() const { return Ctx; }
  /// Returns the language options.
  const LangOptions &getLangOpts() const;
  /// Returns the interpreter stack.
  InterpStack &getStack() { return Stk; }
  /// Returns CHAR_BIT.
  unsigned getCharBit() const;
  /// Return the floating-point semantics for T.
  const llvm::fltSemantics &getFloatSemantics(QualType T) const;

  /// Classifies an expression.
  std::optional<PrimType> classify(QualType T) const;

  const CXXMethodDecl *
  getOverridingFunction(const CXXRecordDecl *DynamicDecl,
                        const CXXRecordDecl *StaticDecl,
                        const CXXMethodDecl *InitialFunction) const;

private:
  /// Runs a function.
  bool Run(State &Parent, const Function *Func, APValue &Result);

  /// Checks a result from the interpreter.
  bool Check(State &Parent, llvm::Expected<bool> &&R);

  /// Current compilation context.
  ASTContext &Ctx;
  /// Interpreter stack, shared across invocations.
  InterpStack Stk;
  /// Constexpr program.
  std::unique_ptr<Program> P;
};

} // namespace interp
} // namespace clang

#endif
