#include "src/eval/builtins.hpp"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "src/eval/value.hpp"

namespace {
void clear_screen() { std::cout << "\033[2J" << std::flush; }
void hide_cursor() { std::cout << "\033[?25l" << std::flush; }
void show_cursor() { std::cout << "\033[?25h" << std::flush; }
void move_cursor(size_t x, size_t y) {
  std::cout << std::format("\033[{};{}H", y, x);
}
}  // namespace

Value native_initscr([[maybe_unused]] const std::vector<Value>& args) {
  clear_screen();
  hide_cursor();
  return Value::make_nil();
}

Value native_deinitscr([[maybe_unused]] const std::vector<Value>& args) {
  clear_screen();
  show_cursor();
  return Value::make_nil();
}

Value native_clear([[maybe_unused]] const std::vector<Value>& args) {
  clear_screen();
  return Value::make_nil();
}

Value native_putchar(const std::vector<Value>& args) {
  if (args.size() != 3) {
    throw std::runtime_error("Wrong number of arguments for putchar!");
  }
  if (!std::all_of(args.begin(), args.end(),
                   [](Value v) { return v.type == ValueType::Number; })) {
    throw std::runtime_error("All of putchar arguments must be numeric!");
  }

  int x = args[0].as.num;
  int y = args[1].as.num;
  char c = args[2].as.num;

  move_cursor(x, y);
  std::cout << c;

  return Value::make_nil();
}
