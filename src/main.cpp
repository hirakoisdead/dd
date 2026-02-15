#include "add.h"
#include "cmd_commit.h"
#include "hash_object.h"
#include "help.h"
#include "init.h"
#include "write_tree.h"
#include <cstdio>
#include <cstring>

typedef struct {
  const char *name;
  const char *help;
  int (*handler)(int, char **);
} command;

command commands[] = {
    {"--help", "Prints usage", help},
    {"init", "Initialises a .dd/ directory", dd_init},
    {"hash-object", "Hashes a given file", hash_object},
    {"add", "adds entries to index", add},
    {"write-tree", "writes tree to object storage", write_tree},
    {"commit", "stores tree snapshots in object storage", commit},
};

int main(int argc, char *argv[]) {
  for (const command &x : commands) {
    if (argc == 1 || strcmp(x.name, argv[1]) == 0) {
      int n_argc = argc - 1;
      char **n_argv = argv + 1;
      if (x.handler(n_argc, n_argv) != 0) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  fprintf(stderr, "Unknown command %s\n", argv[1]);
  return 0;
}
