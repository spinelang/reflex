#include "src/eval/eval.hpp"
#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"
#include <gtest/gtest.h>
#include <type_traits>
#include <utility>

template <typename T> struct is_std_pair : std::false_type {};

template <typename T, typename U>
struct is_std_pair<std::pair<T, U>> : std::true_type {};

template <typename T>
concept isPair = is_std_pair<T>::value;

template <isPair... Globals>
void run(const char *program, int expected, Globals &&...globals) {
  parser::Parser parser(lexer::Lexer{program});
  auto parsed = parser.parse();
  Eval evaluator(parsed);

  (
      [&evaluator](auto &&global) {
        auto &&[name, value] = global;
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

TEST(EvalSuite, SimpleAndOrA) { run("(and (or (and 1 0 1) (and  1 1)) 1)", 1); }

TEST(EvalSuite, SimpleAndOrB) { run("(or (and (or 0 0 0) 0) 0)", 0); }

TEST(EvalSuite, DefineSingle) { run("(define x 12)", 12); }

TEST(EvalSuite, GlobalEnvSingle) { run("x", 119, std::make_pair("x", 119)); }

TEST(EvalSuite, GlobalEnvSimple) {
  run("(* z (+ x y))", 21, std::make_pair("x", 119), std::make_pair("y", -112),
      std::make_pair("z", 3));
}
