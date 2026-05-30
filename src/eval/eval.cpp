#include "src/eval/eval.hpp"
#include "src/eval/env.hpp"
#include "src/parser/ast.hpp"
#include <exception>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <variant>
#include <vector>

int Eval::eval_ast(const parser::Sexp &s, const Env &env) {
  if (std::holds_alternative<parser::Atom>(s.value)) {
    auto &atom = std::get<parser::Atom>(s.value);
    if (std::holds_alternative<parser::Number>(atom.value)) {
      return std::get<parser::Number>(atom.value).num;
    } else if (std::holds_alternative<parser::Symbol>(atom.value)) {
      auto v = env.get(std::get<parser::Symbol>(atom.value).name);
      if (v.has_value()) {
        return v.value();
      } else {
        throw std::runtime_error("unresolved symbol");
      }
    }
  }

  const auto &list = std::get<parser::List>(s.value).list;
  if (list.empty()) {
    throw std::runtime_error("empty lists are not yet supported");
  }

  return eval_list(std::get<parser::List>(s.value), env);
}

int Eval::eval_list(const parser::List &l, const Env &env) {
  if (l.list.empty()) {
    throw std::runtime_error("empty lists not supported");
  }

  if (!std::holds_alternative<parser::Atom>(l.list[0].value) ||
      !std::holds_alternative<parser::Symbol>(
          std::get<parser::Atom>(l.list[0].value).value)) {
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
    if (eval_ast(uargs[0], env)) {
      return eval_ast(uargs[1], env);
    } else {
      return eval_ast(uargs[2], env);
    }
  }

  if (op == "or") {
    for (auto &arg : uargs) {
      if (eval_ast(arg, env) != 0) {
        return 1;
      }
    }
    return 0;
  }

  if (op == "and") {
    for (auto &arg : uargs) {
      if (eval_ast(arg, env) == 0) {
        return 0;
      }
    }
    return 1;
  }

  if (op == "define") {
    if (uargs.size() != 2) {
      throw std::runtime_error("define accepts only two arguments");
    }
    if (!std::holds_alternative<parser::Atom>(uargs[0].value)) {
      throw std::runtime_error("define accepts symbol as first parameter");
    }
    auto atom = std::get<parser::Atom>(uargs[0].value);
    if (!std::holds_alternative<parser::Symbol>(atom.value)) {
      throw std::runtime_error("define accepts symbol as first parameter");
    }
    auto sym = std::get<parser::Symbol>(atom.value);
    auto res = eval_ast(uargs[1], env);

    global_env.add(sym.name, res);

    return res;
  }

  std::vector<int> args;
  for (auto arg : uargs) {
    args.push_back(eval_ast(arg, env));
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

int Eval::eval() {
  global_env.add("pi", 3);
  return eval_ast(ast_, global_env);
}

void Eval::add_global(const std::string &name, int value) {
  global_env.add(name, value);
}
