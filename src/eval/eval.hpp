#pragma once

#include <memory>
#include <vector>

#include "src/eval/env.hpp"
#include "src/parser/ast.hpp"

class Eval {
  parser::Sexp ast_;
  std::shared_ptr<Env> global_env;

  int eval_if(std::span<const parser::Sexp> args,
              const std::shared_ptr<Env>& env);
  int eval_or(std::span<const parser::Sexp> args,
              const std::shared_ptr<Env>& env);
  int eval_and(std::span<const parser::Sexp> args,
               const std::shared_ptr<Env>& env);
  int eval_define(std::span<const parser::Sexp> args,
                  const std::shared_ptr<Env>& env);
  int eval_let(std::span<const parser::Sexp> args,
               const std::shared_ptr<Env>& env);
  int eval_begin(std::span<const parser::Sexp> args,
                 const std::shared_ptr<Env>& env);

  int eval_list(const parser::List& l, std::shared_ptr<Env> env);
  int eval_ast(const parser::Sexp& s, const std::shared_ptr<Env>& env);
  int apply_operator(const std::string& op, const std::vector<parser::Sexp>& l,
                     const std::shared_ptr<Env>& env);

 public:
  Eval(parser::Sexp ast) : ast_(ast), global_env(std::make_shared<Env>()) {}

  int eval();
  void add_global(const std::string& name, int value);
};
