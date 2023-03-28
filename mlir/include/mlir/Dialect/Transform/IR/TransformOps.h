//===- TransformDialect.h - Transform dialect operations --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef MLIR_DIALECT_TRANSFORM_IR_TRANSFORMOPS_H
#define MLIR_DIALECT_TRANSFORM_IR_TRANSFORMOPS_H

#include "mlir/Dialect/PDL/IR/PDLTypes.h"
#include "mlir/Dialect/Transform/IR/TransformInterfaces.h"
#include "mlir/Dialect/Transform/IR/TransformTypes.h"
#include "mlir/IR/FunctionInterfaces.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/IR/OpImplementation.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/IR/SymbolTable.h"
#include "mlir/Interfaces/CallInterfaces.h"
#include "mlir/Interfaces/CastInterfaces.h"
#include "mlir/Interfaces/ControlFlowInterfaces.h"

namespace mlir {
namespace transform {
enum class FailurePropagationMode : uint32_t;
class FailurePropagationModeAttr;

/// A builder function that populates the body of a SequenceOp.
using SequenceBodyBuilderFn = ::llvm::function_ref<void(
    ::mlir::OpBuilder &, ::mlir::Location, ::mlir::BlockArgument)>;
using SequenceBodyBuilderArgsFn =
    ::llvm::function_ref<void(::mlir::OpBuilder &, ::mlir::Location,
                              ::mlir::BlockArgument, ::mlir::ValueRange)>;

/// A listener that updates a TransformState based on IR modifications.
class TrackingListener : public RewriterBase::Listener,
                         public TransformState::Extension {
public:
  explicit TrackingListener(TransformState &state)
      : TransformState::Extension(state) {}

protected:
  virtual Operation *findReplacementOp(Operation *op,
                                       ValueRange newValues) const;

  bool isNewOp(Operation *op) const;

  static Operation *getCommonDefiningOp(ValueRange values);

private:
  void notifyOperationInserted(Operation *op) override;

  void notifyOperationRemoved(Operation *op) override;

  void notifyOperationReplaced(Operation *op, ValueRange newValues) override;

  /// Ops that were newly created during the transform.
  DenseMap<OperationName, DenseSet<Operation *>> newOps;
};

} // namespace transform
} // namespace mlir

#define GET_OP_CLASSES
#include "mlir/Dialect/Transform/IR/TransformOps.h.inc"

#endif // MLIR_DIALECT_TRANSFORM_IR_TRANSFORMOPS_H
