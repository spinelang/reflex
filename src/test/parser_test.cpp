#include "src/parser/parser.hpp"

#include <gtest/gtest.h>

#include "src/lexer/lexer.hpp"

TEST(ParserSuite, Ticks) {
  parser::Parser p(lexer::Lexer("'(1 2 3)"));
  auto res = p.parse();
  ASSERT_NE(res.as_list(), nullptr);
  ASSERT_NE(res.as_list()->list[0].as_atom(), nullptr);
  ASSERT_NE(res.as_list()->list[0].as_atom()->as_sym(), nullptr);
  ASSERT_EQ(res.as_list()->list[0].as_atom()->as_sym()->name, "cons");
  ASSERT_NE(res.as_list()->list[1].as_atom()->as_num(), nullptr);
  ASSERT_EQ(res.as_list()->list[1].as_atom()->as_num()->num, 1);

  ASSERT_NE(res.as_list()->list[2].as_list(), nullptr);
  ASSERT_NE(res.as_list()->list[2].as_list()->list[0].as_atom(), nullptr);
  ASSERT_NE(res.as_list()->list[2].as_list()->list[0].as_atom()->as_sym(),
            nullptr);
  ASSERT_EQ(
      res.as_list()->list[2].as_list()->list[0].as_atom()->as_sym()->name,
      "cons");
  ASSERT_NE(res.as_list()->list[2].as_list()->list[1].as_atom()->as_num(),
            nullptr);
  ASSERT_EQ(res.as_list()->list[2].as_list()->list[1].as_atom()->as_num()->num,
            2);

  ASSERT_NE(res.as_list()->list[2].as_list()->list[2].as_list(), nullptr);
  ASSERT_NE(
      res.as_list()->list[2].as_list()->list[2].as_list()->list[0].as_atom(),
      nullptr);
  ASSERT_NE(res.as_list()
                ->list[2]
                .as_list()
                ->list[2]
                .as_list()
                ->list[0]
                .as_atom()
                ->as_sym(),
            nullptr);
  ASSERT_EQ(res.as_list()
                ->list[2]
                .as_list()
                ->list[2]
                .as_list()
                ->list[0]
                .as_atom()
                ->as_sym()
                ->name,
            "cons");
  ASSERT_NE(res.as_list()
                ->list[2]
                .as_list()
                ->list[2]
                .as_list()
                ->list[1]
                .as_atom()
                ->as_num(),
            nullptr);
  ASSERT_EQ(res.as_list()
                ->list[2]
                .as_list()
                ->list[2]
                .as_list()
                ->list[1]
                .as_atom()
                ->as_num()
                ->num,
            3);
}
