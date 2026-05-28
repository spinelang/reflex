#include "src/eval/eval.hpp"
#include "src/lexer/lexer.hpp"
#include "src/lexer/token.hpp"
#include "src/parser/parser.hpp"
#include <iostream>

int main() {
  auto program = "(+ 12 (* 1 2) (+ 7 (* 12 11)))";
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

  return 0;
}
