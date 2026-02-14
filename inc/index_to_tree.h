#include "index.h"
#include <map>
#include <string>
#include <vector>

struct dir_node {
  std::map<std::string, dir_node> subdirs;
  std::map<std::string, std::string> files; // name,hash
};

dir_node build_node(index &index);
std::vector<std::string> split(const std::string &s, char delim);
std::string write_tree(const dir_node &node);
std::string root_tree_from_index(const index &index);
