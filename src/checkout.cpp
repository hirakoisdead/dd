#include "commit.h"
#include "head.h"
#include "index.h"
#include "object.h"
#include "refs.h"
#include "tree.h"
#include <algorithm>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

void collect_paths(const tree &t, const std::filesystem::path &prefix,
                   std::set<std::filesystem::path> &snapshot_paths) {
  for (const auto &x : t.entries) {
    if (x.type == tree_entry_type::blob) {
      snapshot_paths.emplace(prefix / x.name);
    } else if (x.type == tree_entry_type::tree) {
      tree subtree = load_tree(x.hash);
      collect_paths(subtree, prefix / x.name, snapshot_paths);
    }
  }
}

void remove_paths_not_in_tree(const tree &root_tree,
                              const std::filesystem::path &root) {
  std::set<std::filesystem::path> snapshot_paths;

  collect_paths(root_tree, "", snapshot_paths);

  std::vector<std::filesystem::path> files_to_remove;
  std::vector<std::filesystem::path> dirs;

  for (auto it = std::filesystem::recursive_directory_iterator(root);
       it != std::filesystem::recursive_directory_iterator(); ++it) {

    std::filesystem::path rel = std::filesystem::relative(it->path(), root);

    if (rel == ".dd") {
      it.disable_recursion_pending();
      continue;
    }

    if (it->is_regular_file()) {
      if (snapshot_paths.find(rel) == snapshot_paths.end()) {
        files_to_remove.push_back(it->path());
      }
    }

    if (it->is_directory()) {
      dirs.push_back(it->path());
    }
  }
  for (const auto &path : files_to_remove) {
    std::filesystem::remove(path);
  }

  std::sort(dirs.begin(), dirs.end(), [](const auto &a, const auto &b) {
    return a.string().size() > b.string().size();
  });
  for (const auto &dir : dirs) {
    if (std::filesystem::is_empty(dir)) {
      std::filesystem::remove(dir);
    }
  }
}

void restore_blob(const std::filesystem::path &path, const std::string &hash) {
  object obj = read_object(hash);
  if (obj.type != "blob") {
    throw std::runtime_error("Not a blob object hash");
  }

  std::filesystem::create_directories(path.parent_path());

  FILE *file = fopen(path.c_str(), "wb");

  if (!file) {
    throw std::runtime_error("Couldn't open file");
  }

  fwrite(obj.data.data(), 1, obj.data.size(), file);
}

void restore_tree(const tree &t, const std::filesystem::path &root) {
  for (const auto &entry : t.entries) {
    if (entry.type == tree_entry_type::blob) {
      restore_blob(root / entry.name, entry.hash);
    } else {
      std::filesystem::create_directories(root / entry.name);
      tree subtree = load_tree(entry.hash);

      restore_tree(subtree, root / entry.name);
    }
  }
}

void collect_index_entries(const tree &t, const std::filesystem::path &prefix,
                           Index &index) {
  for (const auto &x : t.entries) {
    if (x.type == tree_entry_type::blob) {
      index_entry entry;
      entry.path = prefix / x.name;
      entry.hash = x.hash;
      index.entries.push_back(entry);
    } else if (x.type == tree_entry_type::tree) {
      tree subtree = load_tree(x.hash);
      collect_index_entries(subtree, prefix / x.name, index);
    } else {
      throw std::runtime_error("Invalid tree entry type");
    }
  }
}

Index build_index_from_tree(const tree &root) {
  Index index;
  collect_index_entries(root, "", index);

  return index;
}

void checkout_commit(const std::string &hash) {
  Commit commit = load_commit(hash);
  tree root = load_tree(commit.tree_hash);

  std::filesystem::path repo_root = std::filesystem::current_path();

  remove_paths_not_in_tree(root, repo_root);
  restore_tree(root, repo_root);
  Index index = build_index_from_tree(root);
  write_index(index);
  set_head_commit(hash);
}

void checkout_branch(const std::string &branch) {
  if (!branch_exists(branch)) {
    throw std::runtime_error("Branch doesn't exist");
  }
  std::string hash = resolve_branch(branch);

  checkout_commit(hash);
  set_head_branch(branch);
}
