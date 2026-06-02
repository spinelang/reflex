#pragma once

#include <memory>
#include <vector>

#include "src/eval/alloc.hpp"
#include "src/eval/env.hpp"
#include "src/eval/value.hpp"
#include "src/parser/ast.hpp"

class Eval {
  parser::Sexp ast_;
  Env global_env;
  BumpAlloc alloc;

  Value eval_if(std::span<const parser::Sexp> args, Env* env);
  Value eval_or(std::span<const parser::Sexp> args, Env* env);
  Value eval_and(std::span<const parser::Sexp> args, Env* env);
  Value eval_define(std::span<const parser::Sexp> args, Env* env);
  Value eval_let(std::span<const parser::Sexp> args, Env* env);
  Value eval_begin(std::span<const parser::Sexp> args, Env* env);
  Value eval_lambda(std::span<const parser::Sexp> args, Env* env);

  Value eval_list(const parser::List& l, Env* env);
  Value eval_ast(const parser::Sexp& s, Env* env);
  Value apply_operator(const std::string& op,
                       const std::vector<parser::Sexp>& l, Env* env);

 public:
  Eval(parser::Sexp ast) : ast_(ast), global_env(Env()) {}

  Value eval();
  void add_global(const std::string& name, Value value);
};
