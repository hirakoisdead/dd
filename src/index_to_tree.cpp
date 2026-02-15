#include "index_to_tree.h"
#include "tree.h"
#include <map>
#include <string>
#include <vector>

dir_node build_node(const Index &index) {
  std::vector<std::string> path_items;
  dir_node node;
  dir_node *node_ptr;

  // iterating throught every index entry
  for (const auto &x : index.entries) {
    node_ptr = &node;
    path_items =
        split(x.path, '/'); // splitting into a list for every entry.path
    if (path_items.size() == 0) {
      throw std::runtime_error("empty entry\n");
    }

    // iterating though that list
    for (size_t i = 0; i < path_items.size() - 1; i++) {
      node_ptr = &node_ptr->subdirs[path_items[i]];
    }

    node_ptr->files[path_items[path_items.size() - 1]] = x.hash;
  }

  return node;
}

// string splitting function
std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(s);
  std::string item;

  while (std::getline(ss, item, delim)) {
    result.push_back(item);
  }

  return result;
}

std::string write_tree(const dir_node &node) {
  tree tree;
  for (const auto &x : node.subdirs) {
    std::string child_hash = write_tree(x.second);
    tree.entries.insert(tree.entries.end(),
                        {x.first, tree_entry_type::tree, child_hash});
  }
  for (const auto &x : node.files) {
    tree.entries.insert(tree.entries.end(),
                        {x.first, tree_entry_type::blob, x.second});
  }
  object tree_obj = serialise_tree(tree);
  std::string tree_hash = write_object(tree_obj);

  return tree_hash;
}

std::string root_tree_from_index(const Index &index) {
  dir_node root_node = build_node(index);
  std::string root_hash = write_tree(root_node);
  return root_hash;
}
