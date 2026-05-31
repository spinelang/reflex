#include <vector>

#include "src/parser/ast.hpp"

namespace parser {
List::List(std::vector<Sexp> l) : list(l) {}
}  // namespace parser
