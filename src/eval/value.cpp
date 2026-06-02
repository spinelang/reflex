#include "src/eval/value.hpp"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const Value v) {
  switch (v.type) {
  case ValueType::Number:
    os << "Number<" << v.as.num << ">";
    break;
  case ValueType::ClosurePtr:
    os << "Closure<" << v.as.closure << ">";
    break;
  case ValueType::ConsPtr:
    os << "Cons<" << v.as.cons->car << ", " << v.as.cons->cdr << ">";
    break;
  case ValueType::Nil:
    os << "Nil";
    break;
  }

  return os;
}
