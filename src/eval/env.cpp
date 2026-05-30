#include "src/eval/env.hpp"
#include <optional>
#include <string>

std::optional<int> Env::get(const std::string &name) const {
  if (entries_.contains(name)) {
    return entries_.at(name);
  } else {
    if (parent_ != nullptr) {
      return parent_->get(name);
    } else {
      return std::nullopt;
    }
  }
}

void Env::add(const std::string &name, int value) { entries_[name] = value; }
