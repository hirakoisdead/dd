#include "branch.h"
#include "refs.h"

int branch(int argc, char *argv[]) {
  if (argc == 1) {
    print_branch_list();
    return 0;
  } else if (argc == 2) {
    create_branch(argv[1]);
    return 0;
  }
  return 1;
}
