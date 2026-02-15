#include "cmd_add.h"
#include "blob.h"
#include "index.h"
#include "object.h"
#include <filesystem>
#include <string>
#include <vector>

int cmd_add(const std::vector<std::filesystem::path> &paths,
            const std::filesystem::path &base) {
  Index index = load_index();
  for (const auto &x : paths) {
    if (!std::filesystem::exists(base / x) ||
        !std::filesystem::is_regular_file(base / x)) {
      return 2;
    }
    object obj = blob_from_file(base / x);
    std::string hash = write_object(obj);
    index_add(index, x, hash);
  }
  write_index(index);
  return 0;
}
