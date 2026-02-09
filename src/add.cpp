#include "add.h"
#include "cmd_add.h"
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>

int add(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }
  std::vector<std::filesystem::path> paths;
  std::filesystem::path base = find_base();
  for (int i = 1; i < argc; i++) {
    bool wrong_path = false;
    std::filesystem::path abs_path = std::filesystem::weakly_canonical(argv[i]);
    std::filesystem::path rel_path = std::filesystem::relative(abs_path, base);
    for (const auto &x : rel_path) {
      if (x == "..") {
        wrong_path = true;
        fprintf(stderr, "path escapes repository");
      }
    }
    if (wrong_path == false) {
      paths.push_back(rel_path);
    }
  }
  if (paths.size() != 0) {
    if (cmd_add(paths, base) != 0) {
      fprintf(stderr, "add failed\n");
      return 2;
    }
  } else {
    fprintf(stderr, "No valid paths");
  }

  return 0;
}

std::filesystem::path find_base() {
  std::filesystem::path cwd = std::filesystem::current_path();
  while (cwd.parent_path() != cwd) {
    if (std::filesystem::exists(cwd / ".dd") &&
        std::filesystem::is_directory(cwd / ".dd")) {
      return cwd;
    } else {
      cwd = cwd.parent_path();
    }
  }

  std::cout << "hihi";
  throw std::runtime_error(
      "not a dd repository(or any of the parent directories)");
}
