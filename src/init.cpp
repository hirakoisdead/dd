#include <cstdio>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int dd_init(int argc, char *argv[]) {
  fs::path cwd = fs::current_path();

  (void)argc;
  (void)argv;

  // Creates the .dd/ directory
  if (fs::exists(".dd")) {
    fprintf(stderr, "dd: repository already initialised\n");
    return 1;
  } else {
    if (!fs::create_directories(".dd")) {
      return 2;
    }
    if (!fs::create_directories(".dd/objects")) {
      fs::remove_all(".dd");
      return 3;
    }
    if (!fs::create_directories(".dd/refs")) {
      fs::remove_all(".dd");
      return 4;
    }
    if (!fs::create_directories(".dd/refs/heads")) {
      fs::remove_all(".dd");
      return 5;
    }
  }

  // Writes to .dd/HEAD
  FILE *HEAD = fopen(".dd/HEAD", "w");
  fprintf(HEAD, "ref: refs/heads/main\n");
  fclose(HEAD);
  // std::ofstream HEAD(".dd/HEAD");
  // HEAD.close();

  // Writes to .dd/config
  FILE *CONF = fopen(".dd/config", "w");
  fprintf(CONF, "[core]\nrepositoryformatversion = 0\n");
  fclose(CONF);
  // std::ofstream CONF(".dd/config");
  // CONF.close();
  printf("Empty repository initialised\n");
  return 0;
}
