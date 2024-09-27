//
// Created by stellaura on 27/09/24.
//
#include "proc_traversal.h"
#include "tree.h"

#include <getopt.h>
#include <iostream>

void print_tree(Tree &tree, bool print_pid, const std::string &sort_by) {
  if (sort_by == "name") {
    tree.sort([](const Node &a, const Node &b) { return a.name < b.name; });
  } else if (sort_by == "pid") {
    tree.sort([](const Node &a, const Node &b) { return a.pid < b.pid; });
  }
  tree.print(print_pid); // 根据 print_pid 控制是否打印 PID
}

int main(int argc, char *argv[]) {
  // 获取进程树数据
  Tree tree = traverse_proc(); // 使用实际的进程数据

  bool print_pid = false;       // 默认不打印 PID
  std::string sort_by = "none"; // 默认不排序

  // 定义长选项
  struct option long_options[] = {
      {"help", no_argument, nullptr, 'h'},
      {"print-pid", no_argument, nullptr, 'p'},
      {"sort", required_argument, nullptr, 's'},
      {nullptr, 0, nullptr, 0} // 结束标记
  };

  int opt;
  while ((opt = getopt_long(argc, argv, "hps:", long_options, nullptr)) != -1) {
    switch (opt) {
    case 'h':
      fmt::println("Usage: {} [options]", argv[0]);
      fmt::println("Options:");
      fmt::println("  -h, --help         Show this help message");
      fmt::println("  -p, --print-pid    Print PID");
      fmt::println("  -s, --sort=NAME    Sort by name (options: name, pid)");
      return 0;

    case 'p':
      print_pid = true; // 设置打印 PID 标志
      break;

    case 's':
      sort_by = optarg; // 获取排序方式
      break;

    default:
      fmt::println("Invalid option. Use -h for help.");
      return 1;
    }
  }

  print_tree(tree, print_pid, sort_by); // 打印树

  return 0;
}