#include "src/parser/ast.hpp"
#include <vector>

namespace parser {
List::List(std::vector<Sexp> l) : list(l) {}
} // namespace parser
