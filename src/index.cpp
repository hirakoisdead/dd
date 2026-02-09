#include "index.h"

index load_index() {
  // TODO
}

void write_index(const index &index) {
  const char *path = ".dd/index";
  FILE *index_fp = fopen(path, "w");
  if (index_fp == NULL) {
    throw std::runtime_error("Couldn't open file");
  }

  for (const auto &x : index.entries) {
    if (fprintf(index_fp, "%s %s\n", x.hash.c_str(), x.path.c_str()) < 0) {
      throw std::runtime_error("Write to index failure");
    }
  }

  if (fclose(index_fp) == EOF) {
    throw std::runtime_error("Failed to close file for write");
  }
}

void index_add(index &index, const std::string &path,
               const std : string &hash) {
  for (auto &x : index.entries) {
    if (x.path == path) {
      x.hash = hash;
      return;
    }
  }
  index_entry entry = {path, hash};
  index.entries.push_back(entry);
}
