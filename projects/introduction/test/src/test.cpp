//
// Created by stellaura on 28/02/24.
//
#include "add.h"
#include "catch2/catch_all.hpp"
#include "fmt/format.h"
#include "multiply.h"
#include <boost/filesystem.hpp>

TEST_CASE("test format") { fmt::println("{} {}", "this is in test", multiply(2, add(-1, 3))); }
TEST_CASE("test boost") {
    namespace fs = boost::filesystem;
    std::string path_to_check = "/home/stellaura/Documents/魔法使いの夜";
    try {
        if (fs::exists(path_to_check)) {
            fmt::print("Path exists: {}\n", path_to_check);
        } else {
            fmt::print("Path does not exist: {}\n", path_to_check);
        }
    } catch (const fs::filesystem_error &ex) {
        fmt::print(stderr, "Error checking path: {}\n", ex.what());
    }
}
TEST_CASE("test"){
    int i = 1;
    const int & cir = i;
    int & ir = i;
    ir = 2;
    REQUIRE(i == 2);
    REQUIRE(cir == 2);
}
