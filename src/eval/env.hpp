#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "src/eval/value.hpp"

class Env {
  // TODO maybe come up with something cooler than that
  std::unordered_map<std::string, Value> entries_;
  Env* parent_;

 public:
  Env(Env* parent = nullptr) : parent_(parent) {};

  std::optional<Value> get(const std::string& name) const;
  void add(const std::string& name, Value value);
};
