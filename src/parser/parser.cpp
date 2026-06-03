#include "src/parser/parser.hpp"

#include <exception>
#include <iostream>
#include <sstream>

#include "src/lexer/lexer.hpp"
#include "src/lexer/token.hpp"
#include "src/parser/ast.hpp"

namespace parser {

Parser::Parser(lexer::Lexer&& lexer)
    : lexer_(std::move(lexer)), cur_(lexer_.scan_token()) {}
void Parser::advance() { cur_ = lexer_.scan_token(); }

void Parser::eat(lexer::TokenType expected_type) {
  if (cur_.type == expected_type) {
    advance();
  } else {
    std::ostringstream oss;
    oss << "unexpected token ";
    oss << cur_.type;

    error(oss.str());
  }
}

void Parser::error(std::string msg) {
  std::cout << "parser error:" << msg << std::endl;
  std::terminate();
}

Sexp Parser::parse_sexp() {
  if (cur_.type == lexer::TokenType::Number) {
    auto val = std::get<int>(cur_.value);
    eat(lexer::TokenType::Number);
    return Sexp(Atom(val));
  }
  if (cur_.type == lexer::TokenType::Symbol) {
    auto val = std::get<std::string>(cur_.value);
    eat(lexer::TokenType::Symbol);
    return Sexp(Atom(val));
  }
  if (cur_.type == lexer::TokenType::Nil) {
    eat(lexer::TokenType::Nil);
    return Sexp(List());
  }

  return Sexp(parse_list());
}

List Parser::parse_list() {
  std::vector<Sexp> l;
  eat(lexer::TokenType::LParen);

  while (1) {
    if (cur_.type == lexer::TokenType::RParen) {
      break;
    }
    l.push_back(parse_sexp());
  }

  eat(lexer::TokenType::RParen);
  return List(l);
}

Sexp Parser::parse() { return parse_sexp(); }

}  // namespace parser
