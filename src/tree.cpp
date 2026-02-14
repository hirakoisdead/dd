#include "tree.h"
#include "object.h"
#include <algorithm>
#include <string>
#include <vector>

object serialise_tree(const tree &tree) {
  object obj;
  std::vector<tree_entry> entries_cpy = tree.entries;
  std::sort(entries_cpy.begin(), entries_cpy.end(), compare_by_name);

  std::vector<uint8_t> data;
  for (const auto &x : entries_cpy) {

    if (x.type == tree_entry_type::blob) {
      data.insert(data.end(), {'b', 'l', 'o', 'b'});
    } else if (x.type == tree_entry_type::tree) {
      data.insert(data.end(), {'t', 'r', 'e', 'e'});
    } else {
      throw std::runtime_error("Invalid tree entry type");
    }

    data.push_back(' ');
    for (size_t i = 0; i < x.name.size(); i++) {
      data.push_back((unsigned char)x.name.data()[i]);
    }
    data.push_back('\0');
    for (size_t i = 0; i < x.hash.size(); i++) {
      data.push_back((unsigned char)x.hash.data()[i]);
    }
  }
  obj.type = "tree";
  obj.data = data;

  return obj;
}

bool compare_by_name(const tree_entry &a, const tree_entry &b) {
  return a.name < b.name;
}
