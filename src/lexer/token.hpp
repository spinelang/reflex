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

  value_type value_;
  // std::string_view lexeme_;
  // int line_;                 // all three are TODO
  // int column_;

public:
  Token(TokenType type, value_type value) : value_(value), type(type) {}

  TokenType type;
  friend std::ostream &operator<<(std::ostream &os, const Token &t);
  bool is_eof() const;
};
} // namespace lexer
