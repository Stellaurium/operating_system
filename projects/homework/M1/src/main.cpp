//
// Created by stellaura on 25/09/24.
//
#include "ordered_list.h"

#include <catch2/catch_all.hpp>
#include <fmt/format.h>

TEST_CASE("ordered list") {
  fmt::println("ordered list:");

  OrderedList<int> list;
  list.insert(10);
  list.insert(4);
  list.insert(2);
  list.insert(5);
  for (auto it = list.begin(); it != list.end(); it++) {
    fmt::println("{}",*it);
  }
  for(auto e:list){
    fmt::println("{}",e);
  }
}

TEST_CASE("tree") { fmt::println("tree:"); }

TEST_CASE("print tree format") { fmt::println("print tree format:"); }

TEST_CASE("proc pid status information") {
  fmt::println("proc pid status information:");
}

TEST_CASE("print pstree") { fmt::println("print pstree:"); }
