#include "src/eval/env.hpp"

#include <optional>
#include <string>

std::optional<Value> Env::get(const std::string& name) const {
  const auto* cur = this;
  while (cur != nullptr) {
    if (cur->entries_.contains(name)) {
      return cur->entries_.at(name);
    }
    cur = cur->parent_.get();
  }

  return std::nullopt;
}

void Env::add(const std::string& name, Value value) { entries_[name] = value; }
