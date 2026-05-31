#include "src/lexer/lexer.hpp"

#include <gtest/gtest.h>

TEST(LexerSuite, ReadChars) {
  auto s = "'a' 'b' 'c' '\n'";
  lexer::Lexer lex(s);
  ASSERT_EQ(lex.scan_token(), 'a');
  ASSERT_EQ(lex.scan_token(), 'b');
  ASSERT_EQ(lex.scan_token(), 'c');
  ASSERT_EQ(lex.scan_token(), '\n');
}

TEST(LexerSuite, CharsClosed) {
  auto s = "'\n' '\n' 'a' '\n'";
  lexer::Lexer lex(s);
  ASSERT_EQ(lex.scan_token(), '\n');
  ASSERT_EQ(lex.scan_token(), '\n');
  ASSERT_EQ(lex.scan_token(), 'a');
  ASSERT_EQ(lex.scan_token(), '\n');
}
