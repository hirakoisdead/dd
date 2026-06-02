#include "commit.h"
#include "object.h"
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <string>

Commit parse_commit_object(const object &obj) {
  if (obj.type != "commit") {
    throw std::runtime_error("object is not a commit");
  }
  Commit commit;
  std::string text(obj.data.begin(), obj.data.end());
  std::istringstream stream(text);
  std::string line;

  while (std::getline(stream, line)) {
    if (line.empty()) {
      break;
    }
    if (line.find("tree ") == 0) {
      commit.tree_hash = line.substr(5);
    } else if (line.find("parent ") == 0) {
      commit.parent_hash = line.substr(7);
    } else if (line.find("author ") == 0) {
      commit.author = line.substr(7);
    } else if (line.find("timestamp ") == 0) {
      commit.timestamp = std::stoll(line.substr(10));
    } else {
      throw std::runtime_error("Unknown commit field");
    }
  }
  if (commit.tree_hash.empty() || commit.author.empty() ||
      std::to_string(commit.timestamp).empty()) {
    throw std::runtime_error("Invalid commit object");
  }
  std::string message;
  bool first = true;
  while (std::getline(stream, line)) {
    if (!first) {
      message += "\n";
    }
    message += line;
    first = false;
  }
  commit.message = message;

  return commit;
}

void print_commit(const std::string &hash, const Commit &commit) {
  printf("commit %s\n", hash.c_str());
  printf("Author: %s\n", commit.author.c_str());

  char time[80];
  std::time_t t = commit.timestamp;
  std::tm *tm = std::localtime(&t);
  std::size_t bytes = std::strftime(time, sizeof(time), "%F %T", tm);
  if (bytes <= 0) {
    throw std::runtime_error("Timestamp error");
  }
  printf("Date: %s\n\n", time);
  std::istringstream stream(commit.message);
  std::string line;
  while (std::getline(stream, line)) {
    printf("    %s", line.c_str());
  }
  printf("\n\n");
}

void walk_commits_from(const std::string &start_hash) {
  std::string current = start_hash;
  while (!current.empty()) {
    object obj = read_object(current);
    Commit c = parse_commit_object(obj);
    print_commit(current, c);
    current = c.parent_hash;
  }
}
