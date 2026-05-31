#pragma once

#include <memory>

#include "src/eval/env.hpp"
#include "src/parser/ast.hpp"

class Eval {
  parser::Sexp ast_;
  Env global_env;

  int eval_list(const parser::List& l, const Env& env);
  int eval_ast(const parser::Sexp& s, const Env& env);

 public:
  Eval(parser::Sexp ast) : ast_(ast), global_env(std::make_shared<Env>()) {}

  int eval();
  void add_global(const std::string& name, int value);
};
