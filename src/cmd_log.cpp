#include "commit.h"
#include "log.h"
#include <cstdio>
#include <string>

int log(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  std::string HEAD = get_current_commit_hash();
  if (HEAD.empty()) {
    printf("No commits yet.\n");
    return 0;
  }
  walk_commits_from(HEAD);
  return 0;
}
