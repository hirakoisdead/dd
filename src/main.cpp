#include "help.h"
#include <cstdio>
#include <cstring>

typedef struct {
  const char *name;
  const char *help;
  int (*handler)(int, char **);
} command;

command commands[] = {{"--help", "Prints usage", help}};

int main(int argc, char *argv[]) {
  for (const command &x : commands) {
    if (argc == 1 || strcmp(x.name, argv[1]) == 0) {
      if (x.handler(argc--, argv++) != 0) {
        return 1;
      } else {
        return 0;
      }
    }
  }
  fprintf(stderr, "Unknown command %s\n", argv[1]);
  return 0;
}
