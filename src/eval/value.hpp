#pragma once

#include <vector>

#include "src/parser/ast.hpp"

class Env;

struct Closure {
  std::vector<std::string> params;
  parser::Sexp body;
  Env* env;
};

struct ConsCell;

enum class ValueType { Number, ClosurePtr, ConsPtr, Nil };
struct Value {
  ValueType type;
  union {
    int num;
    Closure* closure;
    ConsCell* cons;
  } as;

  static Value make_int(int n) { return {ValueType::Number, {.num = n}}; }
  static Value make_closure(Closure* c) {
    return {ValueType::ClosurePtr, {.closure = c}};
  }
  static Value make_cons(ConsCell* c) {
    return {ValueType::ConsPtr, {.cons = c}};
  }

  bool operator==(int n) const {
    return type == ValueType::Number && as.num == n;
  }
};

struct ConsCell {
  Value car;
  Value cdr;
};

std::ostream& operator<<(std::ostream& os, const Value v);
