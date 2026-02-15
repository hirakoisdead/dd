#include "commit.h"
#include "object.h"
#include <cstdint>
#include <cstdio>
#include <string>

object serialise_commit(const Commit &commit) {
  object commit_obj;
  std::vector<uint8_t> commit_data;
  commit_data.insert(commit_data.end(), {'t', 'r', 'e', 'e', ' '});
  for (const auto &x : commit.tree_hash) {
    commit_data.push_back((unsigned char)x);
  }
  commit_data.push_back('\n');

  if (commit.parent_hash.size() != 0) {
    commit_data.insert(commit_data.end(), {'p', 'a', 'r', 'e', 'n', 't', ' '});
    for (const auto &x : commit.parent_hash) {
      commit_data.push_back((unsigned char)x);
    }
    commit_data.push_back('\n');
  }

  commit_data.insert(commit_data.end(), {'a', 'u', 't', 'h', 'o', 'r', ' '});
  for (const auto &x : commit.author) {
    commit_data.push_back((unsigned char)x);
  }
  commit_data.push_back('\n');

  commit_data.insert(commit_data.end(),
                     {'t', 'i', 'm', 'e', 's', 't', 'a', 'm', 'p', ' '});
  std::string str_timestamp = std::to_string(commit.timestamp);
  for (const auto &x : str_timestamp) {
    commit_data.push_back((unsigned char)x);
  }
  commit_data.push_back('\n');
  commit_data.push_back('\n');

  for (const auto &x : commit.message) {
    commit_data.push_back((unsigned char)x);
  }
  commit_data.push_back('\n');

  commit_obj.type = "commit";
  commit_obj.data = commit_data;

  return commit_obj;
}

std::string get_current_branch() {
  char c_branch[64];
  FILE *HEAD = fopen(".dd/HEAD", "r");
  if (!HEAD) {
    throw std::runtime_error("Couldn't open file");
  }

  if (fscanf(HEAD, "ref: refs/heads/%s", c_branch) != 1) {
    throw std::runtime_error("Error reading file");
  }
  std::string branch = c_branch;

  fclose(HEAD);
  return branch;
}

std::string get_current_commit_hash() {
  std::string current = ".dd/refs/heads/";
  current.append(get_current_branch());
  std::filesystem::path current_path = current;

  if (!std::filesystem::exists(current_path)) {
    return "";
  }

  const char *c_current = current.c_str();
  FILE *BRANCH = fopen(c_current, "r");
  if (!BRANCH) {
    throw std::runtime_error("Couldn't open file");
  }
  char c_hash[42];
  if (!fgets(c_hash, 42, BRANCH)) {
    throw std::runtime_error("Error reading file");
  }

  std::string hash = c_hash;
  if (hash.back() == '\n') {
    hash.pop_back();
  }

  fclose(BRANCH);
  return hash;
}

void update_branch_ref(const std::string &branch,
                       const std::string &commit_hash) {
  std::string path = ".dd/refs/heads/";
  path.append(branch);
  std::string tmp = path;
  tmp.append(".tmp");
  const char *c_path = path.c_str();
  const char *c_tmp = tmp.c_str();

  FILE *BRANCH = fopen(c_tmp, "w");
  if (!BRANCH) {
    throw std::runtime_error("Couldn't open file");
  }
  const char *c_commit_hash = commit_hash.c_str();
  fprintf(BRANCH, "%s", c_commit_hash);
  fclose(BRANCH);

  if (rename(c_tmp, c_path) != 0) {
    std::perror("Error renaming file");
  }
}
