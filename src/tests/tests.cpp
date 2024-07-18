// Unit tests
#include "tests.h"

void run_tests () {
  t_get_git_version();
  t_get_remote_names();
  t_get_remote_links();
  t_get_local_branch_names();
  t_get_remote_branch_names();
  t_get_current_branch_name();
  t_get_superproject_working_tree_path();
  t_get_toplevel_path();
}

// Definitions
void t_get_git_version () {
  std::string git_version = get_git_version();

  if (git_version == nullstr)
    std::cout << "t_get_git_version: NULL\n";
  else {
    std::cout << "t_get_git_version: " << git_version << " => ";
    for (const auto& c : git_version) {
      std::cout << int(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_remote_names () {
  std::vector<std::string> remote_names = get_remote_names();

  if (remote_names.empty())
    std::cout << "t_get_remote_names: NULL\n";
  else {
    for (const auto& name : remote_names) {
      std::cout << "t_get_remote_names: " << name << " => ";
      for (const auto& c : name) {
        std::cout << int(c) << ", ";
      } std::cout << std::endl;
    }
  }
}

void t_get_remote_links () {
  std::vector<std::string> remote_names = get_remote_names();

  if (remote_names.empty())
    std::cout << "t_get_remote_links: NULL\n";
  else {
    for (const auto& remote_name : remote_names) {
      std::vector<std::string> remote_push_links = get_remote_links(remote_name, "(push)");
      std::vector<std::string> remote_fetch_links = get_remote_links(remote_name, "(fetch)");

      if (remote_push_links.empty())
        std::cout << "t_get_remote_links: " << remote_name << " (push) NULL\n";
      else {
        std::cout << "t_get_remote_links: " << remote_name << " (push): ";
        for (const auto& link : remote_push_links) {
          std::cout << link << " length: "<< link.length() << " => ";
          for (const auto& c : link) {
            std::cout << int(c) << ", ";
          } std::cout << std::endl;
        }
      }

      if (remote_fetch_links.empty())
        std::cout << "t_get_remote_links: " << remote_name << " (fetch) NULL\n";
      else {
        std::cout << "t_get_remote_links: " << remote_name << " (fetch): ";
        for (const auto& link : remote_fetch_links) {
          std::cout << link << " length: "<< link.length() << " => ";
          for (const auto& c : link) {
            std::cout << int(c) << ", ";
          } std::cout << std::endl;
        }
      }
    }
  }
}

void t_get_local_branch_names () {
  std::vector<std::string> branch_names = get_local_branch_names();

  if (branch_names.empty())
    std::cout << "t_get_local_branch_names: NULL\n";
  else {
    std::cout << "t_get_local_branch_names: ";
    for (const auto& branch_name : branch_names) {
      std::cout << branch_name << " length: "<< branch_name.length() << " => ";
      for (auto c : branch_name) {
        std::cout << int(c) << ", ";
      } std::cout << std::endl;
    }
  }
}

void t_get_remote_branch_names () {
  std::vector<std::string> remote_names = get_remote_names();

  if (remote_names.empty())
    std::cout << "t_get_remote_branch_names: NULL\n";
  else {
    for (const auto& remote_name : remote_names) {
      std::vector<std::string> branch_names = get_remote_branch_names(remote_name);

      if (branch_names.empty())
        std::cout << "t_get_remote_branch_names: " << remote_name << " NULL\n";
      else {
        std::cout << "t_get_remote_branch_names: " << remote_name << " ";
        for (const auto& branch_name : branch_names) {
          std::cout << branch_name << " length: "<< branch_name.length() << " => ";
          for (const auto& c : branch_name) {
            std::cout << int(c) << ", ";
          } std::cout << std::endl;
        }
      }
    }
  }
}

void t_get_current_branch_name () {
  std::string current_branch_name = get_current_branch_name();

  if (current_branch_name == nullstr)
    std::cout << "t_get_current_branch_name: NULL\n";
  else {
    std::cout << "t_get_current_branch_name: " << current_branch_name << " => ";
    for (const auto& c : current_branch_name) {
      std::cout << int(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_superproject_working_tree_path () {
  std::string superproject_working_tree_path = get_superproject_working_tree_path();

  if (superproject_working_tree_path == nullstr)
    std::cout << "t_get_superproject_working_tree_path: NULL\n";
   else {
    std::cout << "t_get_superproject_working_tree_path: " << superproject_working_tree_path << " => ";
    for (const auto& c : superproject_working_tree_path) {
      std::cout << int(c) << ", ";
    } std::cout << std::endl;
  }
}
void t_get_toplevel_path () {
  std::string toplevel_path = get_toplevel_path();

  if (toplevel_path == nullstr)
    std::cout << "t_get_toplevel_path: NULL\n";
   else {
    std::cout << "t_get_toplevel_path: " << toplevel_path << " => ";
    for (const auto& c : toplevel_path) {
      std::cout << int(c) << ", ";
    } std::cout << std::endl;
  }
}