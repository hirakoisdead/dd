#include <string>

std::string read_head();

bool head_is_detached();
void set_head_branch(const std::string &branch);
void set_head_commit(const std::string &hash);
