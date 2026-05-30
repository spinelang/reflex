#include "src/parser/ast.hpp"
#include <iostream>
#include <ostream>
#include <variant>

namespace parser {

std::ostream &operator<<(std::ostream &os, const Number &n) {
  os << "Number(" << n.num << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Symbol &s) {
  os << "Symbol(" << s.name << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const List &l) {
  os << "List(";
  for (auto elem : l.list) {
    os << elem << ",";
  }
  os << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Atom &a) {
  os << "Atom(";
  std::visit([&os](const auto &val) { os << val; }, a.value);
  os << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Sexp &s) {
  os << "Sexp(";
  std::visit([&os](const auto &val) { os << val; }, s.value);
  os << ")";
  return os;
}

void dump_ast(const Sexp &ast, int level) {
  for (int i = 0; i < level; ++i) {
    std::cout << "   ";
  }
  if (std::holds_alternative<parser::Atom>(ast.value)) {
    std::cout << std::get<Atom>(ast.value) << std::endl;
  } else {
    std::cout << "List(" << std::endl;
    for (auto &elem : std::get<List>(ast.value).list) {
      dump_ast(elem, level + 1);
    }
    for (int i = 0; i < level; ++i) {
      std::cout << "   ";
    }
    std::cout << ")" << std::endl;
  }
}

} // namespace parser
