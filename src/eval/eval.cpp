#include "src/eval/eval.hpp"
#include "src/parser/ast.hpp"
#include <exception>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <vector>

int Eval::eval_ast(const parser::Sexp &s) {
  if (s.type == parser::SexpType::Atom) {
    auto &atom = std::get<parser::Atom>(s.value);
    if (atom.type == parser::AtomType::Number) {
      return std::get<parser::Number>(atom.value).num;
    }
    throw std::runtime_error("variables not yet supported");
  }

  const auto &list = std::get<parser::List>(s.value).list;
  if (list.empty()) {
    throw std::runtime_error("empty lists are not yet supported");
  }

  return eval_list(std::get<parser::List>(s.value));
}

int Eval::eval_list(parser::List l) {
  if (l.list.empty()) {
    throw std::runtime_error("empty lists not supported");
  }

  if (l.list[0].type != parser::SexpType::Atom ||
      std::get<parser::Atom>(l.list[0].value).type !=
          parser::AtomType::Symbol) {
    std::cout << "first entry of a list must be a symbol" << std::endl;
    std::terminate();
  }

  auto op =
      std::get<parser::Symbol>(std::get<parser::Atom>(l.list[0].value).value)
          .name;
  auto uargs = l.list | std::views::drop(1);

  if (op == "if") {
    if (uargs.size() != 3) {
      throw std::runtime_error("not accepts only a single argument");
    }
    if (eval_ast(uargs[0])) {
      return eval_ast(uargs[1]);
    } else {
      return eval_ast(uargs[2]);
    }
  }

  std::vector<int> args;
  for (auto arg : uargs) {
    args.push_back(eval_ast(arg));
  }

  if (op == "not") {
    if (args.size() != 1) {
      throw std::runtime_error("not accepts only a single argument");
    }
    return !args[0];
  }

  if (op == "<=") {
    if (args.size() != 2) {
      throw std::runtime_error("<= accepts only two arguments");
    }
    return args[0] <= args[1];
  }

  if (op == "+") {
    auto sum = 0;
    for (auto elem : args) {
      sum += elem;
    }
    return sum;
  }

  if (op == "*") {
    auto prod = 1;
    for (auto elem : args) {
      prod *= elem;
    }
    return prod;
  }

  std::cout << "unsupported operator " << op << std::endl;
  std::terminate();
}

int Eval::eval() { return eval_ast(ast_); }
