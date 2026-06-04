#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

std::string read_head() {
  std::string PATH = ".dd/HEAD";

  std::ifstream file(PATH, std::ios::binary);

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string content(static_cast<std::size_t>(size), '\0');

  file.read(content.data(), size);

  return content;
}

bool head_is_detached() {
  std::string head_data = read_head();
  if (head_data.compare(0, 5, "ref: ") == 0) {
    return false;
  }
  return true;
}

void set_head_branch(const std::string &branch) {
  std::string PATH = ".dd/HEAD";
  const char *c_PATH = PATH.c_str();
  std::string to_write = "ref: refs/heads/" + branch;
  FILE *file = fopen(c_PATH, "w");
  if (!file) {
    throw std::runtime_error("Couldn't open file");
  }
  const char *c_to_write = to_write.c_str();
  fprintf(file, "%s", c_to_write);
  fclose(file);
}

void set_head_commit(const std::string &hash) {
  std::string PATH = ".dd/HEAD";
  const char *c_PATH = PATH.c_str();
  const char *c_hash = hash.c_str();

  FILE *file = fopen(c_PATH, "w");
  if (!file) {
    throw std::runtime_error("Coudn't open file");
  }

  fprintf(file, "%s", c_hash);
  fclose(file);
}
