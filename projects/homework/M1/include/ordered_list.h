//
// Created by stellaura on 25/09/24.
//
#pragma once

#include <concepts>
#include <set>
#include <type_traits>

// 
template <typename T, typename Comp = std::less<T>>
  requires std::is_invocable_r_v<bool, Comp, const T &, const T &>
class OrderedList {
private:
  std::multiset<T, Comp> multi_set_;

public:
  OrderedList() = default;
  OrderedList(Comp comp) : multi_set_(comp) {}

  template <typename U>
    requires std::is_same_v<std::decay_t<U>, T>
  void insert(U &&value) {
    multi_set_.insert(std::forward<U>(value));
  }

  void erase(const T &value) {
    multi_set_.erase(value);
  }

  // 迭代器支持
  using iterator = typename std::multiset<T, Comp>::iterator;
  using const_iterator = typename std::multiset<T, Comp>::const_iterator;

  iterator begin() {
    return multi_set_.begin();
  }

  iterator end() {
    return multi_set_.end();
  }

  const_iterator begin() const {
    return multi_set_.begin();
  }

  const_iterator end() const {
    return multi_set_.end();
  }
};