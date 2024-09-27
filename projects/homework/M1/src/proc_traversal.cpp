//
// Created by stellaura on 26/09/24.
//
#include "proc_traversal.h"
#include "tree.h"

#include <algorithm>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Node parse_status(const fs::path &path) {
  Node node;
  std::ifstream file{path};

  if (!file.is_open()) {
    throw std::runtime_error("无法打开文件: " + path.string());
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.find("Name:", 0) == 0) {
      node.name = line.substr(6); // 去掉 "Name:" 部分
    } else if (line.find("Pid:", 0) == 0) {
      node.pid = std::stoi(line.substr(5)); // 转换为整数
    } else if (line.find("PPid:", 0) == 0) {
      node.ppid = std::stoi(line.substr(6)); // 转换为整数
    }
  }

  file.close();
  return node;
}

Tree traverse_proc() {
  Tree tree;
  fs::path dir = "/proc";

  for (const auto &entry : fs::directory_iterator(dir)) {
    // 检查文件名是否为数字，即是否是 PID 文件夹
    std::string filename = entry.path().filename().string();
    if (std::all_of(filename.begin(), filename.end(), isdigit)) {
      // 拼接出 status 文件的路径
      fs::path status_path = entry.path() / "status";
      auto ret = parse_status(status_path);
      tree.insert(std::move(ret));
    }
  }
  return tree;
}