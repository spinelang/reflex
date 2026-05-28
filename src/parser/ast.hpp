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

  friend std::ostream &operator<<(std::ostream &os, const Symbol &s);
};

struct Number {
  int num;

  Number(int n) : num(n) {}

  friend std::ostream &operator<<(std::ostream &os, const Number &n);
};

struct List {
  std::vector<Sexp> list;

  List(std::vector<Sexp> l);

  friend std::ostream &operator<<(std::ostream &os, const List &l);
};

enum class AtomType { Number, Symbol };
struct Atom {
  AtomType type;
  std::variant<Number, Symbol> value;

  Atom(int n) : type(AtomType::Number), value(n) {}
  Atom(std::string s) : type(AtomType::Symbol), value(s) {}

  friend std::ostream &operator<<(std::ostream &os, const Atom &a);
};

enum class SexpType { Atom, List };
struct Sexp {
  SexpType type;
  std::variant<Atom, List> value;

  Sexp(Atom a) : type(SexpType::Atom), value(a) {}
  Sexp(List l) : type(SexpType::List), value(l) {}

  friend std::ostream &operator<<(std::ostream &os, const Sexp &s);
};

} // namespace parser
