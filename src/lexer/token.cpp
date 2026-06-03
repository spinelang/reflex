#include "src/lexer/token.hpp"

namespace lexer {

std::ostream& operator<<(std::ostream& os, const Token& t) {
  switch (t.type) {
  case TokenType::LParen:
    os << "LP";
    break;
  case TokenType::RParen:
    os << "RP";
    break;
  case TokenType::Number:
    os << "NUM(" << std::get<int>(t.value) << ")";
    break;
  case TokenType::Symbol:
    os << "SYM(" << std::get<std::string>(t.value) << ")";
    break;
  case TokenType::Tick:
    os << "Tick";
    break;
  case TokenType::Nil:
    os << "Nil";
    break;
  case TokenType::Eof:
    os << "Eof";
    break;
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const TokenType t) {
  switch (t) {
  case TokenType::LParen:
    os << "left parenthesis";
    break;
  case TokenType::RParen:
    os << "right parenthesis";
    break;
  case TokenType::Number:
    os << "number";
    break;
  case TokenType::Symbol:
    os << "symbol";
    break;
  case TokenType::Tick:
    os << "tick";
    break;
  case TokenType::Nil:
    os << "nil";
    break;
  case TokenType::Eof:
    os << "eof";
    break;
  }

  return os;
}

bool Token::is_eof() const { return type == TokenType::Eof; }

}  // namespace lexer
