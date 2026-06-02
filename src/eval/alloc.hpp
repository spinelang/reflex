#pragma once

// i just want closures working
#include <cstddef>
#include <memory>
#include <new>
#include <vector>

class BumpAlloc {
  std::size_t size;
  std::byte* start;
  std::byte* next;

  std::vector<std::pair<void (*)(void*), void*>> destructors;

 public:
  BumpAlloc()
      : size(1024 * 1024 * 1024), start(new std::byte[size]), next(start) {}

  ~BumpAlloc() {
    for (auto it = destructors.rbegin(); it != destructors.rend(); ++it) {
      it->first(it->second);
    }
    delete[] start;
  }

  template <typename T, typename... Args>
  T* alloc(Args&&... args) {
    void* raw_next = static_cast<void*>(next);
    auto space = size - (next - start);
    void* aligned = std::align(alignof(T), sizeof(T), raw_next, space);
    if (!aligned) {
      throw std::bad_alloc();
    }
    next = static_cast<std::byte*>(aligned) + sizeof(T);

    auto* obj = new (aligned) T(std::forward<Args>(args)...);

    if constexpr (!std::is_trivially_destructible_v<T>) {
      destructors.push_back(
          {[](void* ptr) { static_cast<T*>(ptr)->~T(); }, obj});
    }
    return obj;
  }
};
