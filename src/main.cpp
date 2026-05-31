#include <iostream>
#include <string>

#include "src/eval/eval.hpp"
#include "src/lexer/lexer.hpp"
#include "src/lexer/token.hpp"
#include "src/parser/ast.hpp"
#include "src/parser/parser.hpp"

void run(const std::string& program) {
  lexer::Lexer prelex(program);

  std::cout << "program:\n" << program << "\n";

  std::cout << "lexing:\n";
  while (1) {
    auto t = prelex.scan_token();
    if (t.is_eof()) {
      break;
    }
    std::cout << t << " ";
  }
  std::cout << std::endl;

  std::cout << "parsing:" << std::endl;
  parser::Parser parser(lexer::Lexer{program});

  auto parsed = parser.parse();
  parser::dump_ast(parsed);
  std::cout << "evaluating:" << std::endl;
  Eval eval(parsed);
  std::cout << eval.eval() << std::endl;
}

int main() {
  std::string input;
  std::cout << "print q to quit" << std::endl;
  while (1) {
    std::getline(std::cin, input);
    if (input == "q") {
      break;
    }
    run(input);
  }
  return 0;
}
