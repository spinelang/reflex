#pragma once

#include "src/parser/ast.hpp"

class Eval {
  parser::Sexp ast_;

  int eval_list(parser::List l);
  int eval_ast(const parser::Sexp &s);

public:
  Eval(parser::Sexp ast) : ast_(ast) {}

  int eval();
};
