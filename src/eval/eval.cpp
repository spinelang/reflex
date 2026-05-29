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

  std::vector<int> args;
  for (auto arg : l.list | std::views::drop(1)) {
    args.push_back(eval_ast(arg));
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
