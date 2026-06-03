#include "src/lexer/lexer.hpp"

#include <gtest/gtest.h>

#include "src/lexer/token.hpp"

TEST(LexerSuite, ReadChars) {
  auto s = "'a' 'b' 'c' '\n'";
  lexer::Lexer lex(s);
  ASSERT_EQ(std::get<int>(lex.scan_token().value), 'a');
  ASSERT_EQ(std::get<int>(lex.scan_token().value), 'b');
  ASSERT_EQ(std::get<int>(lex.scan_token().value), 'c');
  ASSERT_EQ(std::get<int>(lex.scan_token().value), '\n');
}

TEST(LexerSuite, CharsClosed) {
  auto s = "'\n' '\n' 'a' '\n'";
  lexer::Lexer lex(s);
  ASSERT_EQ(std::get<int>(lex.scan_token().value), '\n');
  ASSERT_EQ(std::get<int>(lex.scan_token().value), '\n');
  ASSERT_EQ(std::get<int>(lex.scan_token().value), 'a');
  ASSERT_EQ(std::get<int>(lex.scan_token().value), '\n');
}

TEST(LexerSuite, Nil) {
  auto s = "nil nil nil";
  lexer::Lexer lex(s);
  ASSERT_EQ(lex.scan_token().type, lexer::TokenType::Nil);
  ASSERT_EQ(lex.scan_token().type, lexer::TokenType::Nil);
  ASSERT_EQ(lex.scan_token().type, lexer::TokenType::Nil);
}
