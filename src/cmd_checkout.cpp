#include "refs.h"
#include <checkout.h>
#include <string>

int checkout(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  std::string target = argv[1];

  if (branch_exists(target)) {
    checkout_branch(target);
  } else {
    checkout_commit(target);
  }

  return 0;
}
