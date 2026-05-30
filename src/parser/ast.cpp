#include "src/parser/ast.hpp"
#include <iostream>
#include <ostream>

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
  switch (a.type) {
  case AtomType::Number:
    os << std::get<Number>(a.value);
    break;
  case AtomType::Symbol:
    os << std::get<Symbol>(a.value);
    break;
  }
  os << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Sexp &s) {
  os << "Sexp(";
  switch (s.type) {
  case SexpType::Atom:
    os << std::get<Atom>(s.value);
    break;
  case SexpType::List:
    os << std::get<List>(s.value);
    break;
  }
  os << ")";
  return os;
}

void dump_ast(const Sexp &ast, int level) {
  for (int i = 0; i < level; ++i) {
    std::cout << "   ";
  }
  if (ast.type == SexpType::Atom) {
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
