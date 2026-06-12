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

std::string Lexer::read_sym() {
  auto start = offset_;
  while (peek().has_value() && !is_delimeter(peek().value())) {
    eat();
  }

  return std::string(source_.substr(start, offset_ - start));
}

std::string Lexer::read_string() {
  std::string res;
  eat();  // eat the opening "

  while (peek().has_value() && peek().value() != '\"') {
    auto c = eat().value();
    if (c == '\\') {
      auto esc = eat();
      if (!esc.has_value()) {
        throw std::runtime_error("unexpected eof in escape sequence");
      }

      switch (esc.value()) {
      case 'n':
        res += '\n';
        break;
      case 't':
        res += '\t';
        break;
      case '"':
        res += '"';
        break;
      case '\\':
        res += '\\';
        break;
      }
    } else {
      res += c;
    }
  }

  if (!peek().has_value()) {
    throw std::runtime_error("unterminated string literal");
  }

  eat();  // eat the closing "
  return res;
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

  if (std::isdigit(c) || (c == '-')) {
    if (c == '-') {
      eat();
      if (peek().has_value() && std::isdigit(peek().value())) {
        return Token(TokenType::Number, -read_num());
      } else {
        return Token(TokenType::Symbol, "-");
      }
    }

    return Token(TokenType::Number, read_num());
  } else if (c == '\'') {
    eat();
    auto c = peek();
    if (c.has_value()) {
      if (c == '(') return Token(TokenType::Tick, std::monostate());
      return Token(TokenType::Number, read_char());
    } else {
      throw std::runtime_error("unexpected eof parsing a tick");
    }
    return Token(TokenType::Number, read_char());
  } else if (c == '\"') {
    return Token(TokenType::String, read_string());
  } else {
    auto s = read_sym();
    if (s == "nil") return Token(TokenType::Nil, std::monostate());
    return Token(TokenType::Symbol, std::string(s));
  }
}

}  // namespace lexer
