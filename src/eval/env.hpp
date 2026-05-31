#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

class Env {
  // TODO maybe come up with something cooler than that
  std::unordered_map<std::string, int> entries_;
  std::shared_ptr<Env> parent_;

 public:
  Env(std::shared_ptr<Env> parent = nullptr) : parent_(parent) {};

  std::optional<int> get(const std::string& name) const;
  void add(const std::string& name, int value);
};
