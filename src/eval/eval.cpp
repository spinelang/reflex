#include "src/eval/eval.hpp"
#include "src/parser/ast.hpp"
#include <exception>
#include <iostream>
#include <ranges>

std::variant<parser::Number, parser::Symbol> Eval::eval_atom(parser::Atom a) {
  if (a.type == parser::AtomType::Number) {
    return std::get<parser::Number>(a.value);
  } else {
    return std::get<parser::Symbol>(a.value);
  }
}

int Eval::eval_list(parser::List l) {
  if (l.list.empty()) {
    std::cout << "cannot evaluate empty lists" << std::endl;
    std::terminate();
  }

  if (l.list[0].type != parser::SexpType::Atom ||
      std::get<parser::Atom>(l.list[0].value).type !=
          parser::AtomType::Symbol) {
    std::cout << "first entry of a list must be a symbol" << std::endl;
    std::terminate();
  }

  auto op =
      std::get<parser::Symbol>(std::get<parser::Atom>(l.list[0].value).value)
          .name;
  auto args = l.list | std::views::drop(1);

  if (op == "+") {
    auto sum = 0;
    for (auto elem : args) {
      int t;
      if (elem.type == parser::SexpType::Atom) {
        t = std::get<parser::Number>(std::get<parser::Atom>(elem.value).value)
                .num;
      } else {
        t = eval_list(std::get<parser::List>(elem.value));
      }

      sum += t;
    }
    return sum;
  }
  if (op == "*") {
    auto prod = 1;
    for (auto elem : args) {
      int t;
      if (elem.type == parser::SexpType::Atom) {
        t = std::get<parser::Number>(std::get<parser::Atom>(elem.value).value)
                .num;
      } else {
        t = eval_list(std::get<parser::List>(elem.value));
      }

      prod *= t;
    }
    return prod;
  }

  std::cout << "Unsupported operator " << op << std::endl;
  std::terminate();
}

std::variant<parser::Atom, parser::List> Eval::eval_sexp(parser::Sexp s) {
  return std::get<parser::Atom>(s.value);
}

int Eval::eval() { return eval_list(std::get<parser::List>(ast_.value)); }
