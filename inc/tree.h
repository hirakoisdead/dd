#pragma once

#include "object.h"
#include "tree.h"
#include <string>
#include <vector>

enum class tree_entry_type { blob, tree };

typedef struct {
  std::string name;
  tree_entry_type type;
  std::string hash;
} tree_entry;

typedef struct {
  std::vector<tree_entry> entries;

} tree;

object serialise_tree(const tree &tree);
bool compare_by_name(const tree_entry &a, const tree_entry &b);
