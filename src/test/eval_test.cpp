#include "src/eval/eval.hpp"

#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"

template <typename T>
struct is_std_pair : std::false_type {};

template <typename T, typename U>
struct is_std_pair<std::pair<T, U>> : std::true_type {};

template <typename T>
concept isPair = is_std_pair<T>::value;

template <isPair... Globals>
void run(const char* program, int expected, Globals&&... globals) {
  parser::Parser parser(lexer::Lexer{program});
  auto parsed = parser.parse();
  Eval evaluator(parsed);

  (
      [&evaluator](auto&& global) {
        auto&& [name, value] = global;
        evaluator.add_global(name, value);
      }(std::forward<Globals>(globals)),
      ...);

  ASSERT_EQ(evaluator.eval(), expected);
}

TEST(EvalSuite, One) { run("1", 1); }
TEST(EvalSuite, SimpleArithmetics) { run("(+ 1 2)", 3); }
TEST(EvalSuite, LongArithmetics) {
  run("(* -1 (+ 12 (* 1 2) (+ 7 (* 12 11))))", -153);
}

TEST(EvalSuite, SimpleIfNotLeq) {
  run("(if (not (<= 1 (* 2 12))) (+ 0 0 0 1) (* 1 1 2))", 2);
}
TEST(EvalSuite, SimpleAndOrA) {
  run("(and (or (and 1 0 1) (and  1 1)) 1)", 1);
}
TEST(EvalSuite, SimpleAndOrB) { run("(or (and (or 0 0 0) 0) 0)", 0); }

TEST(EvalSuite, DefineSingle) { run("(define x 12)", 12); }

TEST(EvalSuite, GlobalEnvSingle) {
  run("x", 119, std::make_pair("x", Value::make_int(119)));
}
TEST(EvalSuite, GlobalEnvSimple) {
  run("(* z (+ x y))", 21, std::make_pair("x", Value::make_int(119)),
      std::make_pair("y", Value::make_int(-112)),
      std::make_pair("z", Value::make_int(3)));
}

TEST(EvalSuite, LetSimple) { run("(let (x 119) x)", 119); }
TEST(EvalSuite, LetComplexInit) { run("(let (x (+ 1 (* 2 2))) x)", 5); }
TEST(EvalSuite, LetComplexBody) { run("(let (x 55) (+ 9 (* x 2)))", 119); }
TEST(EvalSuite, LetNested) { run("(let (x 12) (let (y 19) (+ x y)))", 31); }

TEST(EvalSuite, BeginNoSideEffects) { run("(begin 1 2 3 4)", 4); }
TEST(EvalSuite, BeginOneDefine) { run("(begin (define x 21) x)", 21); }
TEST(EvalSuite, BeginDefineVars) {
  run(
      R"(
      (begin
          (define x 10)
          (define y 100)
          (+ x y))
      )",
      110);
}

TEST(EvalSuite, LambdaIdentity) { run("((lambda (x) x) 12)", 12); }
TEST(EvalSuite, LambdaSum) { run("((lambda (x y) (+ x y)) 100 19)", 119); }
TEST(EvalSuite, LambdaNested) {
  run("((lambda (x y) (+ ((lambda (x) (* x 2)) x) y)) 50 19)", 119);
}

TEST(EvalSuite, Fibo10) {
  run(
      R"(
      (begin
          (define fib (lambda (n)
            (if (<= n 2)
              1
              (+
                (fib (+ n -1))
                (fib (+ n -2))))))
  
          (fib 10))
      )",
      55);
}

TEST(EvalSuite, SimpleConsCarCdr) {
  run(R"(
        (begin
          (define con (cons 100 19))
          (+ (car con) (cdr con)))
      )",
      119);
}
TEST(EvalSuite, TwoConsNodes) {
  run(R"(
        (begin
          (define list (cons 1 (cons 2 nil)))
          (+ (car list) (car (cdr list))))
      )",
      3);
}
TEST(EvalSuite, ListSum) {
  run(R"(
        (begin
          (define testlist (cons 1 (cons 2 (cons 3 (cons 4 (cons 5 nil))))))
          (define sum_list
            (lambda (l)
              (if (null? l)
                0
                (+ (car l) (sum_list (cdr l))))))
          (sum_list testlist))
      )",
      15);
}
