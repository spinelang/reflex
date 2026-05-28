#pragma once

#include "src/parser/ast.hpp"
#include <variant>

class Eval {
  parser::Sexp ast_;

  std::variant<parser::Number, parser::Symbol> eval_atom(parser::Atom a);
  int eval_list(parser::List l);
  std::variant<parser::Atom, parser::List> eval_sexp(parser::Sexp s);

public:
  Eval(parser::Sexp ast) : ast_(ast) {}

  int eval();
};
