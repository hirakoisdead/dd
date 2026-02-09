#include <string>
#include <vector>

typedef struct {
  std::string path; // relative to project root
  std::string hash;
} index_entry;

typedef struct {
  std::vector<index_entry> entries;
} index;

index load_index();
void write_index(const index &index);
void index_add(index &index, const std::string &path, const std::string &hash);
