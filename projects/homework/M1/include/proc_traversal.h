//
// Created by stellaura on 26/09/24.
//

#pragma once

#include "filesystem"
#include "tree.h"
namespace fs = std::filesystem;

Node parse_status(const fs::path &path);
Tree traverse_proc();
