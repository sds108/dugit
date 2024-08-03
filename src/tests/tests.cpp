// Unit tests
#include "tests.h"

void run_tests () {
  t_get_shell();
  t_get_ppid();
  t_get_cwd();
  t_get_exec_path();
  t_check_dugit_external_dependencies();
  t_get_git_version();
  t_get_remote_names();
  t_get_remote_links();
  t_get_local_branch_names();
  t_get_remote_branch_names();
  t_get_current_branch_name();
  t_get_superproject_working_tree_path();
  t_get_toplevel_path();
  t_get_superproject_path_manually();
  t_get_toplevel_path_manually();
  t_get_dugit_path();
  t_create_dugit_directory();
  t_add_dugit_to_gitignore();
  t_set_lock_file();
  t_check_lock_file();
  t_unset_lock_file();
  t_fetch_remote();
}

// Definitions
void t_get_shell () {
  std::string shell = get_shell();

  if (shell == nullstr)
    std::cout << "t_get_shell: NULL\n";
  else {
    std::cout << "t_get_shell: " << shell << " => ";
    for (const auto& c : shell) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_ppid () {
  std::string ppid = get_ppid();

  if (ppid == nullstr)
    std::cout << "t_get_ppid: NULL\n";
  else {
    std::cout << "t_get_ppid: " << ppid << " => ";
    for (const auto& c : ppid) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_cwd () {
  std::string cwd = get_cwd();

  if (cwd == nullstr)
    std::cout << "t_get_cwd: NULL\n";
  else {
    std::cout << "t_get_cwd: " << cwd << " => ";
    for (const auto& c : cwd) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_exec_path () {
  std::string git_exec_path = get_executable_path("git");

  if (git_exec_path == nullstr)
    std::cout << "t_get_exec_path: NULL\n";
  else {
    std::cout << "t_get_exec_path: " << git_exec_path << " => ";
    for (const auto& c : git_exec_path) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_git_version () {
  std::string git_version = get_git_version();

  if (git_version == nullstr)
    std::cout << "t_get_git_version: NULL\n";
  else {
    std::cout << "t_get_git_version: " << git_version << " => ";
    for (const auto& c : git_version) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_remote_names () {
  std::string remote_names = get_remote_names(get_cwd());

  if (remote_names == nullstr)
    std::cout << "t_get_remote_names: NULL\n";
  else {
    for (const auto& name : get_lines_from_string(remote_names)) {
      std::cout << "t_get_remote_names: " << name << " => ";
      for (const auto& c : name) {
        std::cout << uint32_t(c) << ", ";
      } std::cout << std::endl;
    }
  }
}

void t_get_remote_links () {
  std::string remote_names = get_remote_names(get_cwd());

  if (remote_names == nullstr)
    std::cout << "t_get_remote_links: NULL\n";
  else {
    for (const auto& remote_name : get_lines_from_string(remote_names)) {
      std::string remote_push_links = get_remote_links(get_cwd(), remote_name, "(push)");
      std::string remote_fetch_links = get_remote_links(get_cwd(), remote_name, "(fetch)");

      if (remote_push_links == nullstr)
        std::cout << "t_get_remote_links: " << remote_name << " (push) NULL\n";
      else {
        std::cout << "t_get_remote_links: " << remote_name << " (push): ";
        for (const auto& link : get_lines_from_string(remote_push_links)) {
          std::cout << link << " length: "<< link.length() << " => ";
          for (const auto& c : link) {
            std::cout << uint32_t(c) << ", ";
          } std::cout << std::endl;
        }
      }

      if (remote_fetch_links == nullstr)
        std::cout << "t_get_remote_links: " << remote_name << " (fetch) NULL\n";
      else {
        std::cout << "t_get_remote_links: " << remote_name << " (fetch): ";
        for (const auto& link : get_lines_from_string(remote_fetch_links)) {
          std::cout << link << " length: "<< link.length() << " => ";
          for (const auto& c : link) {
            std::cout << uint32_t(c) << ", ";
          } std::cout << std::endl;
        }
      }
    }
  }
}

void t_get_local_branch_names () {
  std::string branch_names = get_local_branch_names(get_cwd());

  if (branch_names == nullstr)
    std::cout << "t_get_local_branch_names: NULL\n";
  else {
    std::cout << "t_get_local_branch_names: ";
    for (const auto& branch_name : get_lines_from_string(branch_names)) {
      std::cout << branch_name << " length: "<< branch_name.length() << " => ";
      for (auto c : branch_name) {
        std::cout << uint32_t(c) << ", ";
      } std::cout << std::endl;
    }
  }
}

void t_get_remote_branch_names () {
  std::string remote_names = get_remote_names(get_cwd());

  if (remote_names == nullstr)
    std::cout << "t_get_remote_branch_names: NULL\n";
  else {
    for (const auto& remote_name : get_lines_from_string(remote_names)) {
      std::string branch_names = get_remote_branch_names(get_cwd(), remote_name);

      if (branch_names == nullstr)
        std::cout << "t_get_remote_branch_names: " << remote_name << " NULL\n";
      else {
        std::cout << "t_get_remote_branch_names: " << remote_name << " ";
        for (const auto& branch_name : get_lines_from_string(branch_names)) {
          std::cout << branch_name << " length: "<< branch_name.length() << " => ";
          for (const auto& c : branch_name) {
            std::cout << uint32_t(c) << ", ";
          } std::cout << std::endl;
        }
      }
    }
  }
}

void t_get_current_branch_name () {
  std::string current_branch_name = get_current_branch_name(get_cwd());

  if (current_branch_name == nullstr)
    std::cout << "t_get_current_branch_name: NULL\n";
  else {
    std::cout << "t_get_current_branch_name: " << current_branch_name << " => ";
    for (const auto& c : current_branch_name) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_superproject_working_tree_path () {
  std::string superproject_working_tree_path = get_superproject_working_tree_path(get_cwd());

  if (superproject_working_tree_path == nullstr)
    std::cout << "t_get_superproject_working_tree_path: NULL\n";
   else {
    std::cout << "t_get_superproject_working_tree_path: " << superproject_working_tree_path << " => ";
    for (const auto& c : superproject_working_tree_path) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}
void t_get_toplevel_path () {
  std::string toplevel_path = get_toplevel_path(get_cwd());

  if (toplevel_path == nullstr)
    std::cout << "t_get_toplevel_path: NULL\n";
   else {
    std::cout << "t_get_toplevel_path: " << toplevel_path << " => ";
    for (const auto& c : toplevel_path) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_superproject_path_manually () {
  std::string superproject_working_tree_path = get_superproject_path_manually(get_cwd());

  if (superproject_working_tree_path == nullstr)
    std::cout << "t_get_superproject_path_manually: NULL\n";
   else {
    std::cout << "t_get_superproject_path_manually: " << superproject_working_tree_path << " => ";
    for (const auto& c : superproject_working_tree_path) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_toplevel_path_manually () {
  std::string toplevel_path = get_toplevel_path_manually(get_cwd());

  if (toplevel_path == nullstr)
    std::cout << "t_get_toplevel_path_manually: NULL\n";
   else {
    std::cout << "t_get_toplevel_path_manually: " << toplevel_path << " => ";
    for (const auto& c : toplevel_path) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_get_dugit_path() {
  std::string dugit_path = get_dugit_path(get_cwd());

  if (dugit_path == nullstr)
    std::cout << "t_get_dugit_path: NULL\n";
   else {
    std::cout << "t_get_dugit_path: " << dugit_path << " => ";
    for (const auto& c : dugit_path) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  }
}

void t_create_dugit_directory () {
  std::string dugit_path = get_dugit_path(get_cwd());
  std::string toplevel_path = get_toplevel_path_manually(get_cwd());

  if (dugit_path == nullstr) {
    if (toplevel_path == nullstr)
      std::cout << "t_create_dugit_directory: NULL\n";
    else {
      if (create_dugit_directory(toplevel_path))
        std::cout << "t_create_dugit_directory: " << get_dugit_path(get_cwd()) << std::endl;
      else std::cout << "t_create_dugit_directory: NULL\n";
    }
  } else std::cout << "t_create_dugit_directory: " << get_dugit_path(get_cwd()) << std::endl;
}

void t_add_dugit_to_gitignore () {
  std::string toplevel_path = get_toplevel_path_manually(get_cwd());

  if (toplevel_path == nullstr)
    std::cout << "t_add_dugit_to_gitignore: NULL\n";
  else {
    if (!check_dugit_in_gitignore(toplevel_path)) {
      if (add_dugit_to_gitignore(toplevel_path))
        std::cout << "t_add_dugit_to_gitignore: SUCCESS\n";
      else std::cout << "t_add_dugit_to_gitignore: NULL\n";
    } else std::cout << "t_add_dugit_to_gitignore: SUCCESS\n";
  }
}

void t_check_lock_file () {
  std::string ppid = get_ppid();
  std::string dugit_path = get_dugit_path(get_cwd());

  if (ppid == nullstr || dugit_path == nullstr)
    std::cout << "t_check_lock_file: NULL\n";
  else {
    if (check_lock_file(dugit_path + "/.lock", ppid))
      std::cout << "t_check_lock_file: SUCCESS\n";
    else std::cout << "t_check_lock_file: NULL\n";
  }
}

void t_set_lock_file () {
  std::string ppid = get_ppid();
  std::string dugit_path = get_dugit_path(get_cwd());

  if (ppid == nullstr || dugit_path == nullstr)
    std::cout << "t_set_lock_file: NULL\n";
  else {
    if (set_lock_file(dugit_path + "/.lock", ppid))
      std::cout << "t_set_lock_file: SUCCESS\n";
    else std::cout << "t_set_lock_file: NULL\n";
  }

  // sleep(1);
}

void t_unset_lock_file () {
  std::string dugit_path = get_dugit_path(get_cwd());

  if (dugit_path == nullstr)
    std::cout << "t_unset_lock_file: NULL\n";
  else {
    if (unset_lock_file(dugit_path + "/.lock"))
      std::cout << "t_unset_lock_file: SUCCESS\n";
    else std::cout << "t_unset_lock_file: NULL\n";
  }
}

void t_check_dugit_external_dependencies () {
  if (check_dugit_external_dependencies())
    std::cout << "t_check_dugit_external_dependencies: SUCCESS\n";
  else std::cout << "t_check_dugit_external_dependencies: NULL\n";
}

void t_fetch_remote () {
  std::string remote_names = get_remote_names(get_cwd());
  std::string current_branch_name = get_current_branch_name(get_cwd());

  if (current_branch_name == nullstr || remote_names == nullstr)
    std::cout << "t_fetch_remote: NULL\n";
  else {
    for (const auto& remote_name : get_lines_from_string(remote_names)) {
      if (fetch_remote(get_cwd(), remote_name, current_branch_name))
        std::cout << "t_fetch_remote: " << remote_name << "/" << current_branch_name << " SUCCESS"<< std::endl;
      else std::cout << "t_fetch_remote: " << remote_name << "/" << current_branch_name << " NULL"<< std::endl;
    }
  }
}
