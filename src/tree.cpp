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

tree parse_tree_object(const object &obj) {
  if (obj.type != "tree") {
    throw std::runtime_error("Object is not a tree");
  }

  tree tree;

  size_t pos = 0;

  while (pos < obj.data.size()) {

    size_t start = pos;
    while (pos < obj.data.size() && obj.data[pos] != ' ') {
      pos++;
    }
    if (pos == obj.data.size()) {
      throw std::runtime_error("Invalid tree object");
    }

    std::string type(obj.data.begin() + static_cast<std::ptrdiff_t>(start),
                     obj.data.begin() + static_cast<std::ptrdiff_t>(pos));
    pos++;

    start = pos;
    while (pos < obj.data.size() && obj.data[pos] != '\0') {
      pos++;
    }
    if (pos == obj.data.size()) {
      throw std::runtime_error("Invalid tree object");
    }

    std::string name(obj.data.begin() + static_cast<std::ptrdiff_t>(start),
                     obj.data.begin() + static_cast<std::ptrdiff_t>(pos));
    pos++;

    std::string hash(obj.data.begin() + static_cast<std::ptrdiff_t>(pos),
                     obj.data.begin() + static_cast<std::ptrdiff_t>(pos) + 40);
    pos += 40;

    tree_entry entry;
    entry.name = name;
    entry.hash = hash;
    if (type == "blob") {
      entry.type = tree_entry_type::blob;
    } else if (type == "tree") {
      entry.type = tree_entry_type::tree;
    } else {
      throw std::runtime_error("Invalid object type");
    }

    tree.entries.push_back(entry);
  }

  return tree;
}

tree load_tree(const std::string &hash) {
  object obj = read_object(hash);
  return parse_tree_object(obj);
}
