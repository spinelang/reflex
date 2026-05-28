#include "src/parser/ast.hpp"
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
} // namespace parser
