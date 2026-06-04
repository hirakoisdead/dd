#include "index.h"
#include "tree.h"
#include <filesystem>
#include <string>

void collect_paths(const tree &tree, const std::filesystem::path &prefix);
void remove_paths_not_in_tree(const tree &root_tree,
                              const std::filesystem::path &root);
void restore_blob(const std::filesystem::path &path, const std::string &hash);
void restore_tree(const tree &tree, const std::filesystem::path &root);
void collect_index_entries(const tree &tree,
                           const std::filesystem::path &prefix, Index &index);
Index build_index_from_tree(const tree &root);
void checkout_commit(const std::string &hash);
void checkout_branch(const std::string &branch);
