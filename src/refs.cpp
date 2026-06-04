#include <filesystem>
#include <fstream>
#include <string>

std::string resolve_branch(const std::string &branch) {
  std::string PATH = ".dd/refs/heads/" + branch;

  std::ifstream file(PATH, std::ios::binary);

  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string hash(static_cast<std::size_t>(size), '\0');
  file.read(hash.data(), size);

  return hash;
}

bool branch_exists(const std::string &branch) {
  std::filesystem::path PATH = ".dd/refs/heads/" + branch;
  return std::filesystem::exists(PATH) &&
         std::filesystem::is_regular_file(PATH);
}
