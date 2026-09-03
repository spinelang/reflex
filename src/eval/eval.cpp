#include "src/eval/eval.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <exception>
#include <iostream>
#include <ranges>
#include <span>
#include <stdexcept>
#include <vector>

#include "src/eval/builtins.hpp"
#include "src/eval/env.hpp"
#include "src/eval/value.hpp"
#include "src/parser/ast.hpp"

Value Eval::eval_ast(const parser::Sexp& s, Env* env) {
  const auto* atom = s.as_atom();

  if (atom != nullptr) {
    const auto* num = atom->as_num();
    if (num != nullptr) {
      return Value::make_int(num->num);
    }

    const auto* sym = atom->as_sym();
    if (sym != nullptr) {
      const auto& v = env->get(sym->name);
      if (v.has_value()) {
        return v.value();
      } else {
        throw std::runtime_error("unresolved symbol: " + sym->name);
      }
    }
  }

  const auto list = s.as_list();
  if (list->list.empty()) {
    return Value::make_nil();
  }

  return eval_list(*list, env);
}

Value Eval::eval_if(std::span<const parser::Sexp> args, Env* env) {
  if (args.size() != 3) {
    throw std::runtime_error("not accepts only a single argument");
  }
  auto cond = eval_ast(args[0], env);
  if (cond.type != ValueType::Number) {
    throw std::runtime_error("if's condition must be a numerical");
  }
  if (cond.as.num) {
    return eval_ast(args[1], env);
  } else {
    return eval_ast(args[2], env);
  }
}

Value Eval::eval_or(std::span<const parser::Sexp> args, Env* env) {
  for (const auto& arg : args) {
    auto cond = eval_ast(arg, env);
    if (cond.type != ValueType::Number) {
      throw std::runtime_error("or's condition must be a numerical");
    }
    if (cond.as.num) {
      return Value::make_int(1);
    }
  }
  return Value::make_int(0);
}

Value Eval::eval_and(std::span<const parser::Sexp> args, Env* env) {
  for (const auto& arg : args) {
    auto cond = eval_ast(arg, env);
    if (cond.type != ValueType::Number) {
      throw std::runtime_error("or's condition must be a numerical");
    }
    if (!cond.as.num) {
      return Value::make_int(0);
    }
  }
  return Value::make_int(1);
}

Value Eval::eval_define(std::span<const parser::Sexp> args, Env* env) {
  if (args.size() != 2) {
    throw std::runtime_error("define accepts only two arguments");
  }

  const auto& sym = args[0].as_atom() ? args[0].as_atom()->as_sym() : nullptr;
  if (!sym) {
    throw std::runtime_error("define accepts symbol as first parameter");
  }

  const auto res = eval_ast(args[1], env);

  global_env.add(sym->name, res);

  return res;
}

Value Eval::eval_let(std::span<const parser::Sexp> args, Env* env) {
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

  auto* new_env = alloc.alloc<Env>(env);
  new_env->add(sym->name, eval_ast(value_sexp, env));

  return eval_ast(args[1], new_env);
}

Value Eval::eval_begin(std::span<const parser::Sexp> args, Env* env) {
  for (const auto& arg : args | std::views::take(args.size() - 1)) {
    eval_ast(arg, env);
  }
  return eval_ast(args.back(), env);
}

Value Eval::eval_list(const parser::List& l, Env* env) {
  const auto* pop =
      l.list[0].as_atom() ? l.list[0].as_atom()->as_sym() : nullptr;
  if (pop) {
    const auto& op = pop->name;
    std::span<const parser::Sexp> args(l.list.data() + 1, l.list.size() - 1);

    if (op == "if") return eval_if(args, env);
    if (op == "or") return eval_or(args, env);
    if (op == "and") return eval_and(args, env);
    if (op == "define") return eval_define(args, env);
    if (op == "let") return eval_let(args, env);
    if (op == "begin") return eval_begin(args, env);
    if (op == "lambda") return eval_lambda(args, env);

    if (op == "+" || op == "*" || op == "/" || op == "-" || op == "<=" ||
        op == "not" || op == "cons" || op == "car" || op == "cdr" ||
        op == "null?") {
      return apply_operator(op, l.list, env);
    }
  }

  std::vector<Value> args;
  for (const auto& arg : l.list | std::views::drop(1)) {
    args.push_back(eval_ast(arg, env));
  }

  auto oc = eval_ast(l.list[0], env);
  if (oc.type == ValueType::ClosurePtr) {
    auto closure = oc.as.closure;
    Env* call_env = alloc.alloc<Env>(closure->env);

    for (size_t i = 0; i < closure->params.size(); ++i) {
      call_env->add(closure->params[i], args[i]);
    }

    return eval_ast(closure->body, call_env);
  } else if (oc.type == ValueType::NativePtr) {
    return oc.as.native_func(args);
  } else {
    throw std::runtime_error("not a form");
  }
}

Value Eval::eval_lambda(std::span<const parser::Sexp> args, Env* env) {
  auto* closure = alloc.alloc<Closure>();
  const auto* param_list = args[0].as_list();
  if (!param_list) {
    throw std::runtime_error("lambda accepts only lists as first parameter");
  }

  for (auto v : param_list->list) {
    const auto* name = v.as_atom() ? v.as_atom()->as_sym() : nullptr;
    if (!name) throw std::runtime_error("lambda parameters must be symbols");
    closure->params.push_back(name->name);
  }

  closure->body = args[1];

  closure->env = env;

  return Value::make_closure(closure);
}

Value Eval::apply_operator(const std::string& op,
                           const std::vector<parser::Sexp>& l, Env* env) {
  std::vector<Value> args;
  for (const auto& arg : l | std::views::drop(1)) {
    args.push_back(eval_ast(arg, env));
  }

  if (op == "not") {
    if (args.size() != 1) {
      throw std::runtime_error("not accepts only a single argument");
    }
    if (args[0].type != ValueType::Number) {
      throw std::runtime_error("not accepts only integers");
    }
    return Value::make_int(!args[0].as.num);
  }

  if (op == "<=") {
    if (args.size() != 2) {
      throw std::runtime_error("<= accepts only two arguments");
    }
    if (args[0].type != ValueType::Number ||
        args[1].type != ValueType::Number) {
      throw std::runtime_error("<= accepts only integers");
    }
    return Value::make_int(args[0].as.num <= args[1].as.num);
  }

  if (op == "+") {
    auto sum = 0;
    for (const auto& elem : args) {
      if (elem.type != ValueType::Number) {
        throw std::runtime_error("+ accepts only numbers");
      }
      sum += elem.as.num;
    }
    return Value::make_int(sum);
  }

  if (op == "*") {
    auto prod = 1;
    for (const auto& elem : args) {
      if (elem.type != ValueType::Number) {
        throw std::runtime_error("* accepts only numbers");
      }
      prod *= elem.as.num;
    }
    return Value::make_int(prod);
  }

  if (op == "-") {
    if (args[0].type != ValueType::Number) {
      throw std::runtime_error("- accepts only numbers");
    }
    auto res = args[0].as.num;
    for (const auto& elem : args | std::views::drop(1)) {
      if (elem.type != ValueType::Number) {
        throw std::runtime_error("- accepts only numbers");
      }
      res -= elem.as.num;
    }
    return Value::make_int(res);
  }

  if (op == "/") {
    if (args.size() != 2) {
      throw std::runtime_error("/ accepts only two arguments");
    }
    if (args[0].type != ValueType::Number) {
      throw std::runtime_error("/ accepts only numbers");
    }
    if (args[1].type != ValueType::Number) {
      throw std::runtime_error("/ accepts only numbers");
    }
    if (args[1].as.num == 0) {
      throw std::runtime_error("division by zero!");
    }
    return Value::make_int(args[0].as.num / args[1].as.num);
  }

  if (op == "cons") {
    if (args.size() != 2) {
      throw std::runtime_error("cons takes exactly two arguments");
    }
    auto* con = alloc.alloc<ConsCell>();
    con->car = args[0];
    con->cdr = args[1];

    return Value::make_cons(con);
  }

  if (op == "car") {
    if (args.size() != 1) {
      throw std::runtime_error("car takes exactly one argument");
    }
    if (args[0].type != ValueType::ConsPtr) {
      throw std::runtime_error("car's argument must be a cons cell");
    }

    return args[0].as.cons->car;
  }

  if (op == "cdr") {
    if (args.size() != 1) {
      throw std::runtime_error("cdr takes exactly one argument");
    }
    if (args[0].type != ValueType::ConsPtr) {
      throw std::runtime_error("cdr's argument must be a cons cell");
    }

    return args[0].as.cons->cdr;
  }

  if (op == "null?") {
    if (args.size() != 1) {
      throw std::runtime_error("null? takes exactly one argument");
    }
    return Value::make_int(args[0].type == ValueType::Nil);
  }

  std::cout << "unsupported operator " << op << std::endl;
  std::terminate();
}

Value Eval::eval() {
  global_env.add("pi", Value::make_int(3));
  global_env.add("clear", Value::make_native(native_clear));
  global_env.add("putchar", Value::make_native(native_putchar));
  return eval_ast(ast_, &global_env);
}

void Eval::add_global(const std::string& name, Value value) {
  global_env.add(name, value);
}
