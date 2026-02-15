#pragma once

#include <string>
#include <vector>

typedef struct {
  std::string path; // relative to project root
  std::string hash;
} index_entry;

typedef struct {
  std::vector<index_entry> entries;
} Index;

Index load_index();
void write_index(const Index &index);
void index_add(Index &index, const std::string &path, const std::string &hash);
