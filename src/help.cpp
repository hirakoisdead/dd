#include <cstdio>

int help(int argc, char *argv[]) {
  printf("usage: ./build/dd [--help] [init]\n");

  // Suppressing unused var errors for now
  (void)argc;
  (void)argv;

  return 0;
}
