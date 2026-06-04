#include "hash.h"
#include "object.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static void test_hash_determinism() {
  std::vector<uint8_t> data = {'h', 'e', 'l', 'l', 'o'};

  std::string h1 = hash_bytes(data);
  std::string h2 = hash_bytes(data);

  assert(h1 == h2);
}

static void test_hash_sensitivity() {
  std::vector<uint8_t> a = {'h', 'e', 'l', 'l', 'o'};
  std::vector<uint8_t> b = {'h', 'e', 'l', 'l', 'o', '!'};

  assert(hash_bytes(a) != hash_bytes(b));
}

static void test_object_serialization_affects_hash() {
  object a;
  a.type = "blob";
  a.data = {'x'};

  object b;
  b.type = "tree";
  b.data = {'x'};

  std::string ha = write_object(a);
  std::string hb = write_object(b);

  assert(ha != hb);
}

static void test_object_written_once() {
  object obj;
  obj.type = "blob";
  obj.data = {'h', 'i'};

  std::string h1 = write_object(obj);
  std::string h2 = write_object(obj);

  assert(h1 == h2);

  auto path = object_path_from_hash(h1);
  assert(std::filesystem::exists(path));
}

static void test_object_persistence() {
  object obj;
  obj.type = "blob";
  obj.data = {'p', 'e', 'r', 's', 'i', 's', 't'};

  std::string hash = write_object(obj);

  object read = read_object(hash);

  assert(read.type == obj.type);
  assert(read.data == obj.data);
}

static void test_corruption_detection() {
  object obj;
  obj.type = "blob";
  obj.data = {'b', 'a', 'd'};

  std::string hash = write_object(obj);
  auto path = object_path_from_hash(hash);

  // Corrupt the object file
  std::ofstream out(path, std::ios::app | std::ios::binary);
  out.put('X');
  out.close();

  bool caught = false;
  try {
    read_object(hash);
  } catch (...) {
    caught = true;
  }

  assert(caught);
}

int main() {
  std::cout << "Running Phase 2 object store tests...\n";

  test_hash_determinism();
  test_hash_sensitivity();
  test_object_serialization_affects_hash();
  test_object_written_once();
  test_object_persistence();
  test_corruption_detection();

  std::cout << "All Phase 2 tests passed.\n";
  return 0;
}
