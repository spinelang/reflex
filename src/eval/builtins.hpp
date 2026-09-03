#include <vector>

#include "src/eval/value.hpp"

Value native_initscr(const std::vector<Value>& args);
Value native_clear(const std::vector<Value>& args);
Value native_deinitscr(const std::vector<Value>& args);
Value native_putchar(const std::vector<Value>& args);
