#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "src/eval/value.hpp"

class Env {
  // TODO maybe come up with something cooler than that
  std::unordered_map<std::string, Value> entries_;
  std::shared_ptr<Env> parent_;

 public:
  Env(std::shared_ptr<Env> parent = nullptr) : parent_(parent) {};
  Env(const Env&) = delete;
  Env& operator=(const Env&) = delete;

  std::optional<Value> get(const std::string& name) const;
  void add(const std::string& name, Value value);
};
