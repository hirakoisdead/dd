#include "commit.h"
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

void write_branch(const std::string &name, const std::string &hash) {
  std::filesystem::path path = std::string(".dd/refs/heads/") + name;
  std::ofstream file(path);
  if (!file) {
    throw std::runtime_error("Failed to open file");
  }

  file << hash;
}

bool is_valid_branch_name(const std::string &name) {
  static const std::regex pattern("^[a-zA-Z0-9_-]+$");

  return std::regex_match(name, pattern);
}

std::vector<std::string> list_branches() {
  std::vector<std::string> branches;

  for (const auto &entry :
       std::filesystem::directory_iterator(".dd/refs/heads")) {

    if (entry.is_regular_file()) {
      branches.push_back(entry.path().filename().string());
    }
  }
  std::sort(branches.begin(), branches.end());
  return branches;
}

void print_branch_list() {
  auto branches = list_branches();
  std::string current = get_current_branch();
  for (const auto &branch : branches) {
    if (branch == current) {
      printf("* ");
    } else {
      printf("  ");
    }
    printf("%s\n", branch.c_str());
  }
}
