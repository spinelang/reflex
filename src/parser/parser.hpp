#pragma once

#include "src/lexer/lexer.hpp"
#include "src/lexer/token.hpp"
#include "src/parser/ast.hpp"

namespace parser {

class Parser {
  lexer::Lexer lexer_;
  lexer::Token cur_;

  void advance();
  void eat(lexer::TokenType expected_type);

  void error(std::string msg);

  Sexp parse_sexp();
  List parse_list();
  Atom parse_atom();
  Symbol parse_symbol();
  Number parse_number();

public:
  Parser(lexer::Lexer lexer);
  Sexp parse();
};
} // namespace parser
