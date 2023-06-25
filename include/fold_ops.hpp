#pragma once

namespace ctl {
  struct fold_ops {};

  struct logical_or : public fold_ops {};
  struct logical_and : public fold_ops {};
  struct bitwise_or : public fold_ops {};
  struct bitwise_and : public fold_ops {};
  struct sum : public fold_ops {};
  struct diff : public fold_ops {};
  struct multiply : public fold_ops {};
  struct divide : public fold_ops {};
  struct modulo : public fold_ops {};
  // struct xor;
  struct left_shift : public fold_ops {};
  struct right_shift : public fold_ops {};
  struct greater_than : public fold_ops {};
  struct less_than : public fold_ops {};
  struct not_equal : public fold_ops {};
}