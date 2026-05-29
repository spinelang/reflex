#include "src/eval/eval.hpp"
#include "src/lexer/lexer.hpp"
#include "src/lexer/token.hpp"
#include "src/parser/parser.hpp"
#include <iostream>

void run(const char *program, int expected) {
  lexer::Lexer lex(program);
  parser::Parser parser(lex);
  auto parsed = parser.parse();
  Eval eval(parsed);

  std::cout << "program:\n" << program << "\n";

  std::cout << "lexing:\n";
  while (1) {
    auto t = lex.scan_token();
    if (t.is_eof()) {
      break;
    }
    std::cout << t << " ";
  }
  std::cout << std::endl;

  std::cout << "parsing:" << std::endl;
  std::cout << parsed << std::endl;
  std::cout << "evaluating:" << std::endl;
  std::cout << eval.eval() << std::endl;

  std::cout << "expected: " << expected << std::endl;
}

int main() {
  run("(* -1 (+ 12 (* 1 2) (+ 7 (* 12 11))))", -153);

  run("(if (not (<= 1 (* 2 12))) (+ 0 0 0 1) (* 1 1 2))", 2);

  return 0;
}
