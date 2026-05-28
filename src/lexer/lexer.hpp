#pragma once

#include "src/lexer/token.hpp"
#include <cctype>
#include <optional>
#include <string_view>

namespace lexer {

class Lexer {
  std::string_view source_;
  std::size_t offset_;

  std::optional<char> peek();
  std::optional<char> eat();

  void skip_whitespace();
  std::string_view read_sym();
  int read_num();

public:
  Lexer(std::string_view src) : source_(src), offset_(0) {}

  Token scan_token();
};

} // namespace lexer
