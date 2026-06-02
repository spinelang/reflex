#include "src/lexer/lexer.hpp"

#include <cctype>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

#include "src/lexer/token.hpp"

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

bool is_delimeter(char c) { return std::isspace(c) || c == ')' || c == '('; }

void Lexer::skip_whitespace() {
  while (peek().has_value() && std::isspace(peek().value())) {
    eat();
  }
}

std::string_view Lexer::read_sym() {
  auto start = offset_;
  while (peek().has_value() && !is_delimeter(peek().value())) {
    eat();
  }

  return source_.substr(start, offset_ - start);
}

int Lexer::read_num() {
  auto start = offset_;
  if (peek() == '-') eat();
  while (peek().has_value() && std::isdigit(peek().value())) {
    eat();
  }

  return std::stoi(std::string(source_.substr(start, offset_ - start)));
}

int Lexer::read_char() {
  eat();
  auto co = eat();
  if (!co.has_value()) {
    throw std::runtime_error("empty char literal");
  }
  auto c = co.value();
  char res;

  if (c != '\\') {
    res = c;
  } else {
    switch (eat().value()) {
    case 'n':
      res = '\n';
      break;
    case 't':
      res = '\t';
      break;
    default:
      throw std::runtime_error("unknown escape sequence");
    }
  }

  eat();
  return static_cast<int>(res);
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
  } else if (c == '\'') {
    return Token(TokenType::Number, read_char());
  } else {
    auto s = read_sym();
    return Token(TokenType::Symbol, std::string(s));
  }
}

}  // namespace lexer
