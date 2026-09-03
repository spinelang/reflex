#pragma once

#include <vector>

#include "src/parser/ast.hpp"

class Env;
struct Value;

struct Closure {
  std::vector<std::string> params;
  parser::Sexp body;
  Env* env;
};

using NativePtr = Value (*)(const std::vector<Value>& args);

struct ConsCell;

enum class ValueType { Number, ClosurePtr, NativePtr, ConsPtr, Nil };
struct Value {
  ValueType type;
  union {
    int num;
    Closure* closure;
    ConsCell* cons;
    NativePtr native_func;
  } as;

  static Value make_int(int n) { return {ValueType::Number, {.num = n}}; }
  static Value make_closure(Closure* c) {
    return {ValueType::ClosurePtr, {.closure = c}};
  }
  static Value make_native(NativePtr fn) {
    return {ValueType::NativePtr, {.native_func = fn}};
  }
  static Value make_cons(ConsCell* c) {
    return {ValueType::ConsPtr, {.cons = c}};
  }
  static Value make_nil() { return {ValueType::Nil, {.cons = nullptr}}; }

  bool operator==(int n) const {
    return type == ValueType::Number && as.num == n;
  }
};

struct ConsCell {
  Value car;
  Value cdr;
};

std::ostream& operator<<(std::ostream& os, const Value v);
