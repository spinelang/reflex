#include "src/lexer/token.hpp"

namespace lexer {

std::ostream &operator<<(std::ostream &os, const Token &t) {
  switch (t.type_) {
  case TokenType::LParen:
    os << " LP ";
    break;
  case TokenType::RParen:
    os << " RP ";
    break;
  case TokenType::Number:
    os << "NUM(" << std::get<int>(t.value_) << ")";
    break;
  case TokenType::Symbol:
    os << "SYM(" << std::get<std::string>(t.value_) << ")";
    break;
  case TokenType::Eof:
    os << "Eof";
    break;
  }

  return os;
}

bool Token::is_eof() const { return type_ == TokenType::Eof; }

} // namespace lexer
