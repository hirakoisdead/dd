#include "write_tree.h"
#include "index.h"
#include "index_to_tree.h"
#include <iostream>
#include <string>

int write_tree(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  Index index = load_index();
  std::string hash = root_tree_from_index(index);
  std::cout << hash;
  return 0;
}
