#pragma once

#include <ostream>
#include <variant>

namespace lexer {

enum class TokenType { LParen, RParen, Number, Symbol, Eof };

class Token {
  using value_type = std::variant<int,           // Number
                                  std::string,   // Symbol
                                  std::monostate // all others
                                  >;

  // std::string_view lexeme_;
  // int line_;                 // all three are TODO
  // int column_;

public:
  Token(TokenType type, value_type value) : type(type), value(value) {}

  TokenType type;
  value_type value;

  friend std::ostream &operator<<(std::ostream &os, const Token &t);
  bool is_eof() const;
};

std::ostream &operator<<(std::ostream &os, const TokenType t);

} // namespace lexer
