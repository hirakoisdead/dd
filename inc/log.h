#include "commit.h"
#include "object.h"
#include <string>

Commit parse_commit_object(const object &obj);
void print_commit(const std::string &hash, const Commit &commit);
void walk_commits_from(const std::string &start_hash);
