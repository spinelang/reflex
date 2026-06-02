#include "src/eval/eval.hpp"

#include <exception>
#include <iostream>
#include <memory>
#include <ranges>
#include <span>
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

int Eval::eval_if(std::span<const parser::Sexp> args,
                  const std::shared_ptr<Env>& env) {
  if (args.size() != 3) {
    throw std::runtime_error("not accepts only a single argument");
  }
  if (eval_ast(args[0], env)) {
    return eval_ast(args[1], env);
  } else {
    return eval_ast(args[2], env);
  }
}

int Eval::eval_or(std::span<const parser::Sexp> args,
                  const std::shared_ptr<Env>& env) {
  for (const auto& arg : args) {
    if (eval_ast(arg, env) != 0) {
      return 1;
    }
  }
  return 0;
}

int Eval::eval_and(std::span<const parser::Sexp> args,
                   const std::shared_ptr<Env>& env) {
  for (const auto& arg : args) {
    if (eval_ast(arg, env) == 0) {
      return 0;
    }
  }
  return 1;
}

int Eval::eval_define(std::span<const parser::Sexp> args,
                      const std::shared_ptr<Env>& env) {
  if (args.size() != 2) {
    throw std::runtime_error("define accepts only two arguments");
  }

  const auto& sym = args[0].as_atom() ? args[0].as_atom()->as_sym() : nullptr;
  if (!sym) {
    throw std::runtime_error("define accepts symbol as first parameter");
  }

  const auto res = eval_ast(args[1], env);

  global_env->add(sym->name, res);

  return res;
}

int Eval::eval_let(std::span<const parser::Sexp> args,
                   const std::shared_ptr<Env>& env) {
  if (args.size() != 2) {
    throw std::runtime_error("let accepts only two arguments");
  }

  const auto* pinit_list = args[0].as_list();
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

  return eval_ast(args[1], new_env);
}

int Eval::eval_begin(std::span<const parser::Sexp> args,
                     const std::shared_ptr<Env>& env) {
  for (const auto& arg : args | std::views::take(args.size() - 1)) {
    eval_ast(arg, env);
  }
  return eval_ast(args.back(), env);
}

int Eval::eval_list(const parser::List& l, std::shared_ptr<Env> env) {
  const auto* pop =
      l.list[0].as_atom() ? l.list[0].as_atom()->as_sym() : nullptr;
  if (!pop) {
    // TODO subject to change with the addition of lambdas
    throw std::runtime_error("first entry of a list must be a symbol");
  }

  const auto& op = pop->name;
  std::span<const parser::Sexp> args(l.list.data() + 1, l.list.size() - 1);

  if (op == "if") return eval_if(args, env);
  if (op == "or") return eval_or(args, env);
  if (op == "and") return eval_and(args, env);
  if (op == "define") return eval_define(args, env);
  if (op == "let") return eval_let(args, env);
  if (op == "begin") return eval_begin(args, env);

  return apply_operator(op, l.list, env);
}

int Eval::apply_operator(const std::string& op,
                         const std::vector<parser::Sexp>& l,
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
