#include "src/eval/eval.hpp"
#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"
#include <gtest/gtest.h>

void run(const char *program, int expected) {
  parser::Parser parser(lexer::Lexer{program});
  auto parsed = parser.parse();
  Eval eval(parsed);

  ASSERT_EQ(eval.eval(), expected);
}

TEST(EvalSuite, One) { run("1", 1); }

TEST(EvalSuite, SimpleArithmetics) { run("(+ 1 2)", 3); }

TEST(EvalSuite, LongArithmetics) {
  run("(* -1 (+ 12 (* 1 2) (+ 7 (* 12 11))))", -153);
}

TEST(EvalSuite, SimpleIfNotLeq) {
  run("(if (not (<= 1 (* 2 12))) (+ 0 0 0 1) (* 1 1 2))", 2);
}
