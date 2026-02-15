#include "cmd_commit.h"
#include "commit.h"
#include "index.h"
#include "index_to_tree.h"
#include "object.h"
#include <cstring>
#include <string>

int commit(int argc, char *argv[]) {
  if (argc < 3 || strcmp(argv[1], "-m") != 0) {
    return 1;
  }
  Commit commit;
  Index index = load_index();
  if (index.entries.empty()) {
    return 2;
  }
  std::string root_hash = root_tree_from_index(index);
  std::string parent_hash = get_current_commit_hash();

  if (!parent_hash.empty()) {
    object parent_obj = read_object(parent_hash);
    std::string data(parent_obj.data.begin(), parent_obj.data.end());
    std::istringstream stream(data);
    std::string first_line;
    std::getline(stream, first_line);
    std::string parent_tree_hash = first_line.substr(5);
    if (root_hash == parent_tree_hash) {
      fprintf(stderr, "Nothing to commit");
      return 3;
    }
  }

  commit.tree_hash = root_hash;
  commit.parent_hash = parent_hash;
  commit.author = "muaviz";
  commit.timestamp = std::time(nullptr);
  commit.message = argv[2];

  object obj = serialise_commit(commit);
  std::string commit_hash = write_object(obj);
  update_branch_ref(get_current_branch(), commit_hash);

  return 0;
}
