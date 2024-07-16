// Unit tests
#include "tests.h"

void run_tests () {
  t_get_git_version();
  t_get_remote_names();
}

// Definitions
void t_get_git_version () {
  std::string git_version = get_git_version();

  if (git_version == nullstr)
    std::cout << "t_get_git_version: NULL\n";

  else {
    std::cout << "t_get_git_version: " << git_version << std::endl;

    for (auto c : git_version) {
      std::cout << int(c) << " " << std::endl;
    }
  }
}

void t_get_remote_names () {
  std::vector<std::string> remote_names = get_remote_names();

  if (remote_names.empty())
    std::cout << "t_get_remote_names: NULL\n";

  else {
    std::cout << "t_get_git_version: ";
    for (auto name : remote_names) {
      std::cout << name << ", ";
    } std::cout << std::endl;
  }
}