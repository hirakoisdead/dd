#include "object.h"
#include <cstdint>
#include <string>

typedef struct {
  std::string tree_hash;
  std::string parent_hash;
  std::string author;
  std::string message;
  std::int64_t timestamp;
} Commit;

object serialise_commit(const Commit &commit);
std::string get_current_branch();
std::string get_current_commit_hash();
void update_branch_ref(const std::string &branch,
                       const std::string &commit_hash);
