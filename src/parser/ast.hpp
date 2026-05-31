#pragma once

#include <string>
#include <variant>
#include <vector>

namespace parser {

struct Program;
struct Sexp;
struct List;
struct Atom;
struct Symbol;
struct Number;

struct Symbol {
  std::string name;

  Symbol(std::string name) : name(name) {}

  friend std::ostream& operator<<(std::ostream& os, const Symbol& s);
};

struct Number {
  int num;

  Number(int n) : num(n) {}

  friend std::ostream& operator<<(std::ostream& os, const Number& n);
};

struct List {
  std::vector<Sexp> list;

  List(std::vector<Sexp> l);

  friend std::ostream& operator<<(std::ostream& os, const List& l);
};

struct Atom {
  std::variant<Number, Symbol> value;

  Atom(int n) : value(n) {}
  Atom(std::string s) : value(s) {}

  friend std::ostream& operator<<(std::ostream& os, const Atom& a);
};

struct Sexp {
  std::variant<Atom, List> value;

  Sexp(Atom a) : value(a) {}
  Sexp(List l) : value(l) {}

  friend std::ostream& operator<<(std::ostream& os, const Sexp& s);
};

void dump_ast(const Sexp& ast, int level = 0);
}  // namespace parser
