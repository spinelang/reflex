#include "src/lexer/lexer.hpp"
#include "src/lexer/token.hpp"
#include <cctype>
#include <optional>
#include <string>
#include <variant>

namespace lexer {

std::optional<char> Lexer::peek() {
  if (offset_ >= source_.size()) {
    return std::nullopt;
  }
  return source_.at(offset_);
}

std::optional<char> Lexer::eat() {
  if (offset_ >= source_.size()) {
    return std::nullopt;
  }
  return source_.at(offset_++);
}

void Lexer::skip_whitespace() {
  while (peek().has_value() && std::isspace(peek().value())) {
    eat();
  }
}

std::string_view Lexer::read_sym() {
  auto start = offset_;
  while (peek().has_value() && !std::isspace(peek().value())) {
    eat();
  }

  return source_.substr(start, offset_ - start);
}

int Lexer::read_num() {
  auto start = offset_;
  while (peek().has_value() && std::isdigit(peek().value())) {
    eat();
  }

  return std::stoi(std::string(source_.substr(start, offset_ - start)));
}

Token Lexer::scan_token() {
  skip_whitespace();
  auto oc = peek();

  if (!oc.has_value()) {
    return Token(TokenType::Eof, std::monostate{});
  }

  auto c = oc.value();

  switch (c) {
  case '(':
    eat();
    return Token(TokenType::LParen, std::monostate{});
  case ')':
    eat();
    return Token(TokenType::RParen, std::monostate{});
  }

  if (std::isdigit(c) || c == '-') {
    auto num = read_num();
    return Token(TokenType::Number, num);
  } else {
    auto s = read_sym();
    return Token(TokenType::Symbol, std::string(s));
  }
}

} // namespace lexer
