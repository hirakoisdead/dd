#include <filesystem>
#include <string>
#include <vector>

void write_branch(const std::string &name, const std::string &hash);
bool is_valid_branch_name(const std::string &name);
std::vector<std::string> list_branches();
void print_branch_list();
