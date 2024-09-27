//
// Created by stellaura on 26/09/24.
//

#pragma once

#include "filesystem"
#include "tree.h"
#include <optional>

namespace fs = std::filesystem;

std::optional<Node> parse_status(const fs::path &path);
Tree traverse_proc();
