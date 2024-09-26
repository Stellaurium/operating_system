//
// Created by stellaura on 25/09/24.
//

#pragma once
#include <fmt/format.h>
#include <list>
#include <map>
#include <string>

// 功能需求
// O(1)的插入，支持使用自定义comp进行排序

struct Node {
  int pid;
  int ppid;
  std::string name;
};

class Tree {
private:
  // node 只用来记录节点信息
  // 节点之间的关系是 map 维护
  // 在插入一个节点时，父亲节点可能还没有生成node
  // 这样分离之后，可以直接在父亲节点的map中插入子节点
  // 不使用vector (node delete)
  std::map<int, Node> node_map;
  std::map<int, std::list<int>> parent_map;

  void print_tab_(int pid, int level = 0) {
    const auto &node = node_map[pid];
    if (pid != 0) {
      for (int i = 0; i < level; i++) {
        fmt::print("    ");
      }
      fmt::println("{}({})", node.name, node.pid);
    }
    for (auto children_pid : parent_map[pid]) {
      print_tab_(children_pid, level + 1);
    }
  }

  void print_utf8_(int pid, std::string begin = "") {
    const auto &node = node_map[pid];
    fmt::print("{}({})", node.name, node.pid);

    // 对齐开头
    int space_count = node.name.length() + count_digits_(pid) + 2;
    for (int i = 0; i < space_count; i++) {
      begin += ' ';
    }

    int length = parent_map[pid].size();
    int count = 0;
    auto it = parent_map[pid].begin();

    if (length == 0) {
      fmt::println("");
      return;
    }

    while (count < length) {
      std::string next_begin = begin;

      if (count != 0) {
        fmt::print("{}", begin);
      }

      // only one child
      if (length == 1 && count == 0) {
        next_begin += "   ";
        fmt::print("───");
      }
      // first child
      else if (count == 0) {
        next_begin += " │ ";
        fmt::print("─┬─");
      }
      // last child
      else if (count == length - 1) {
        next_begin += " │ ";
        fmt::print(" └─");
      } else {
        next_begin += " │ ";
        fmt::print(" ├─");
      }

      print_utf8_(*it, next_begin);

      it++;
      count++;
    }
  }

  int count_digits_(int number) {
    int count = 0;
    if (number == 0) {
      return 1;
    }
    while (number != 0) {
      number /= 10;
      count++;
    }
    return count;
  }

public:
  Tree() { node_map[0] = Node{0, -1, "root"}; }

  void insert(int pid, int ppid, std::string name) {
    node_map[pid] = Node{pid, ppid, name};
    parent_map[ppid].push_back(pid);
  }

  // 这个任务好像根本不需要erase
  void erase(int pid) {
    int ppid = node_map[pid].ppid;
    auto &siblings = parent_map[ppid];
    siblings.remove(pid);
    node_map.erase(pid);
  }

  template <typename Comp>
    requires std::is_invocable_r_v<bool, Comp, const Node &, const Node &>
  void sort(Comp comp) {
    for (auto &[pid, list] : parent_map) {
      auto comp_int = [&](int a, int b) {
        return comp(node_map[a], node_map[b]);
      };
      list.sort(comp_int);
    }
  }

  void print_tab() { print_tab_(0, -1); }
  void print_utf8() { print_utf8_(0, ""); }
  void print() { print_utf8(); }
};
