#include "src/eval/eval.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <vector>

#include "src/eval/env.hpp"
#include "src/parser/ast.hpp"

int Eval::eval_ast(const parser::Sexp& s, const std::shared_ptr<Env>& env) {
  const auto* atom = s.as_atom();

  if (atom != nullptr) {
    const auto* num = atom->as_num();
    if (num != nullptr) {
      return num->num;
    }

    const auto* sym = atom->as_sym();
    if (sym != nullptr) {
      const auto& v = env->get(sym->name);
      if (v.has_value()) {
        return v.value();
      } else {
        throw std::runtime_error("unresolved symbol");
      }
    }
  }

  const auto list = s.as_list();
  if (list->list.empty()) {
    throw std::runtime_error("empty lists are not yet supported");
  }

  return eval_list(*list, env);
}

int Eval::eval_list(const parser::List& l, std::shared_ptr<Env> env) {
  const auto* pop =
      l.list[0].as_atom() ? l.list[0].as_atom()->as_sym() : nullptr;
  if (!pop) {
    // TODO subject to change with the addition of lambdas
    throw std::runtime_error("first entry of a list must be a symbol");
  }

  const auto& op = pop->name;
  const auto& uargs = l.list | std::views::drop(1);

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
    for (const auto& arg : uargs) {
      if (eval_ast(arg, env) != 0) {
        return 1;
      }
    }
    return 0;
  }

  if (op == "and") {
    for (const auto& arg : uargs) {
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

    const auto& sym =
        uargs[0].as_atom() ? uargs[0].as_atom()->as_sym() : nullptr;
    if (!sym) {
      throw std::runtime_error("define accepts symbol as first parameter");
    }

    const auto res = eval_ast(uargs[1], env);

    global_env->add(sym->name, res);

    return res;
  }

  if (op == "let") {
    if (uargs.size() != 2) {
      throw std::runtime_error("let accepts only two arguments");
    }

    const auto* pinit_list = uargs[0].as_list();
    if (!pinit_list) {
      throw std::runtime_error("let accepts list as first parameter");
    }

    const auto& init_list = pinit_list->list;
    if (init_list.size() != 2) {
      throw std::runtime_error(
          "identifier list of let must have only two entries");
    }
    const auto& sym =
        init_list[0].as_atom() ? init_list[0].as_atom()->as_sym() : nullptr;
    if (!sym) {
      throw std::runtime_error(
          "first element of lets initializer list must be a symbol");
    }

    const auto& value_sexp = init_list[1];

    auto new_env = std::make_shared<Env>(env);
    new_env->add(sym->name, eval_ast(value_sexp, env));

    return eval_ast(uargs[1], new_env);
  }

  if (op == "begin") {
    for (const auto& arg : uargs | std::views::take(uargs.size() - 1)) {
      eval_ast(arg, env);
    }
    return eval_ast(uargs.back(), env);
  }

  return apply_operator(op, l.list, env);
}

int Eval::apply_operator(const std::string& op,
                         const std::vector<parser::Sexp> l,
                         const std::shared_ptr<Env>& env) {
  std::vector<int> args;
  for (const auto& arg : l | std::views::drop(1)) {
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
    for (const auto& elem : args) {
      sum += elem;
    }
    return sum;
  }

  if (op == "*") {
    auto prod = 1;
    for (const auto& elem : args) {
      prod *= elem;
    }
    return prod;
  }

  std::cout << "unsupported operator " << op << std::endl;
  std::terminate();
}

int Eval::eval() {
  global_env->add("pi", 3);
  return eval_ast(ast_, global_env);
}

void Eval::add_global(const std::string& name, int value) {
  global_env->add(name, value);
}
