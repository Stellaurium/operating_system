//
// Created by stellaura on 25/09/24.
//
#include "tree.h"

#include <catch2/catch_all.hpp>
#include <fmt/format.h>

template <typename T>
using MultisetOrderedList = std::multiset<T>;


TEST_CASE("tree") {
  fmt::println("=======================\ntree:");
  Tree tree;
  tree.insert(3,0,"first app");
  tree.insert(4,3,"2 A");
  tree.insert(11,4,"3 D");
  tree.insert(10,4,"3 E");
  tree.insert(6,3,"2 C");
  tree.insert(5,3,"2 B");
  tree.insert(14,0,"second app");

  fmt::println("normal:");
  tree.print_utf8();

//  fmt::println("=======================\nsorted by name:");
//  tree.sort([](const Node &a, const Node &b) { return a.name < b.name; });
//  tree.print_utf8();
//
//  fmt::println("=======================\nsorted by pid:");
//  tree.sort([](const Node &a, const Node &b) { return a.pid < b.pid; });
//  tree.print_utf8();
}

TEST_CASE("proc pid status information") {
  fmt::println("proc pid status information:");
}

TEST_CASE("print_tab pstree") { fmt::println("print_tab pstree:"); }
