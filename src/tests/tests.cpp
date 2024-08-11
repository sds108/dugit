// Unit tests
#include "tests.h"

void run_tests () {
  t_get_shell();
  t_get_ppid();
  t_get_cwd();
  t_get_pwd();
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
  char* shell = get_shell(); // envariables do not have to be freed

  if (shell == NULL) {
    std::cout << "t_get_shell: NULL\n";
    return;
  }

  std::cout << "t_get_shell: " << shell << " => ";
  for (const auto& c : std::string(shell)) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
}

void t_get_ppid () {
  std::string* ppid = get_ppid();

  if (ppid == NULL) {
    std::cout << "t_get_ppid: NULL\n";
    return;
  }

  std::cout << "t_get_ppid: " << *ppid << " => ";
  for (const auto& c : *ppid) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(ppid);
}

void t_get_cwd () {
  std::string* cwd = get_cwd();

  if (cwd == NULL) {
    std::cout << "t_get_cwd: NULL\n";
    return;
  }

  std::cout << "t_get_cwd: " << *cwd << " => ";
  for (const auto& c : *cwd) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(cwd);
}

void t_get_pwd () {
  std::string* pwd = get_pwd();

  if (pwd == NULL) {
    std::cout << "t_get_pwd: NULL\n";
    return;
  }

  std::cout << "t_get_pwd: " << *pwd << " => ";
  for (const auto& c : *pwd) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(pwd);
}

void t_get_exec_path () {
  std::string* git_exec_path = get_executable_path("git");

  if (git_exec_path == NULL) {
    std::cout << "t_get_exec_path: NULL\n";
    return;
  }

  std::cout << "t_get_exec_path: " << *git_exec_path << " => ";
  for (const auto& c : *git_exec_path) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(git_exec_path);
}

void t_get_git_version () {
  std::string* git_version = get_git_version();

  if (git_version == NULL) {
    std::cout << "t_get_git_version: NULL\n";
    return;
  }

  std::cout << "t_get_git_version: " << *git_version << " => ";
  for (const auto& c : *git_version) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(git_version);
}

void t_get_remote_names () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_remote_names: NULL\n";
    return;
  }

  std::string* remote_names = get_remote_names(*cwd);
  delete(cwd);
  if (remote_names == NULL) {
    std::cout << "t_get_remote_names: NULL\n";
    return;
  }

  for (const auto& name : get_lines_from_string(*remote_names)) {
    std::cout << "t_get_remote_names: " << name << " => ";
    for (const auto& c : name) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  } delete(remote_names);
}

void t_get_remote_links () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_remote_links: NULL\n";
    return;
  }

  std::string* remote_names = get_remote_names(*cwd);
  if (remote_names == NULL) {
    std::cout << "t_get_remote_links: NULL\n";
    delete(cwd);
    return;
  }

  for (const auto& remote_name : get_lines_from_string(*remote_names)) {
    std::string* remote_push_links = get_remote_links(*cwd, remote_name, "(push)");
    if (remote_push_links == NULL) {
      std::cout << "t_get_remote_links: " << remote_name << " (push) NULL\n";
    } else {
      std::cout << "t_get_remote_links: " << remote_name << " (push): ";
      for (const auto& link : get_lines_from_string(*remote_push_links)) {
        std::cout << link << " length: "<< link.length() << " => ";
        for (const auto& c : link) {
          std::cout << uint32_t(c) << ", ";
        } std::cout << std::endl;
      } delete(remote_push_links);
    }

    std::string* remote_fetch_links = get_remote_links(*cwd, remote_name, "(fetch)");
    if (remote_fetch_links == NULL) {
      std::cout << "t_get_remote_links: " << remote_name << " (fetch) NULL\n";
    } else {
      std::cout << "t_get_remote_links: " << remote_name << " (fetch): ";
      for (const auto& link : get_lines_from_string(*remote_fetch_links)) {
        std::cout << link << " length: "<< link.length() << " => ";
        for (const auto& c : link) {
          std::cout << uint32_t(c) << ", ";
        } std::cout << std::endl;
      } delete(remote_fetch_links);
    }
  }

  delete(cwd);
  delete(remote_names);
}

void t_get_local_branch_names () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_local_branch_names: NULL\n";
    return;
  }

  std::string* branch_names = get_local_branch_names(*cwd);
  delete(cwd);
  if (branch_names == NULL) {
    std::cout << "t_get_local_branch_names: NULL\n";
    return;
  }

  std::cout << "t_get_local_branch_names: ";
  for (const auto& branch_name : get_lines_from_string(*branch_names)) {
    std::cout << branch_name << " length: "<< branch_name.length() << " => ";
    for (auto c : branch_name) {
      std::cout << uint32_t(c) << ", ";
    } std::cout << std::endl;
  } delete(branch_names);
}

void t_get_remote_branch_names () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_remote_branch_names: NULL\n";
    return;
  }

  std::string* remote_names = get_remote_names(*cwd);
  if (remote_names == NULL) {
    delete(cwd);
    std::cout << "t_get_remote_branch_names: NULL\n";
    return;
  }

  for (const auto& remote_name : get_lines_from_string(*remote_names)) {
    std::string* branch_names = get_remote_branch_names(*cwd, remote_name);

    if (branch_names == NULL) {
      std::cout << "t_get_remote_branch_names: " << remote_name << " NULL\n";
      continue;
    }

    std::cout << "t_get_remote_branch_names: " << remote_name << " ";
    for (const auto& branch_name : get_lines_from_string(*branch_names)) {
      std::cout << branch_name << " length: "<< branch_name.length() << " => ";
      for (const auto& c : branch_name) {
        std::cout << uint32_t(c) << ", ";
      } std::cout << std::endl;
    } delete(branch_names);
  }
  
  delete(remote_names);
  delete(cwd);
}

void t_get_current_branch_name () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_current_branch_name: NULL\n";
    return;
  }

  std::string* current_branch_name = get_current_branch_name(*cwd);
  delete(cwd);
  if (current_branch_name == NULL) {
    std::cout << "t_get_current_branch_name: NULL\n";
    return;
  }

  std::cout << "t_get_current_branch_name: " << *current_branch_name << " => ";
  for (const auto& c : *current_branch_name) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(current_branch_name);
}

void t_get_superproject_working_tree_path () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_superproject_working_tree_path: NULL\n";
    return;
  }

  std::string* superproject_working_tree_path = get_superproject_working_tree_path(*cwd);
  delete(cwd);
  if (superproject_working_tree_path == NULL) {
    std::cout << "t_get_superproject_working_tree_path: NULL\n";
    return;
  }

  std::cout << "t_get_superproject_working_tree_path: " << *superproject_working_tree_path << " => ";
  for (const auto& c : *superproject_working_tree_path) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(superproject_working_tree_path);
}
void t_get_toplevel_path () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_toplevel_path: NULL\n";
    return;
  }

  std::string* toplevel_path = get_toplevel_path(*cwd);
  if (toplevel_path == NULL) {
    delete(cwd);
    std::cout << "t_get_toplevel_path: NULL\n";
    return;
  }

  std::cout << "t_get_toplevel_path: " << *toplevel_path << " => ";
  for (const auto& c : *toplevel_path) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(cwd);
  delete(toplevel_path);
}

void t_get_superproject_path_manually () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_superproject_path_manually: NULL\n";
    return;
  }

  std::string* superproject_working_tree_path = get_superproject_path_manually(*cwd);
  delete(cwd);
  if (superproject_working_tree_path == NULL) {
    std::cout << "t_get_superproject_path_manually: NULL\n";
    return;
  }

  std::cout << "t_get_superproject_path_manually: " << *superproject_working_tree_path << " => ";
  for (const auto& c : *superproject_working_tree_path) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(superproject_working_tree_path);
}

void t_get_toplevel_path_manually () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_toplevel_path_manually: NULL\n";
    return;
  }

  std::string* toplevel_path = get_toplevel_path_manually(*cwd);
  delete(cwd);
  if (toplevel_path == NULL) {
    std::cout << "t_get_toplevel_path_manually: NULL\n";
    return;
  }

  std::cout << "t_get_toplevel_path_manually: " << *toplevel_path << " => ";
  for (const auto& c : *toplevel_path) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(toplevel_path);
}

void t_get_dugit_path() {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_get_dugit_path: NULL\n";
    return;
  }

  std::string* dugit_path = get_dugit_path(*cwd);
  delete(cwd);
  if (dugit_path == NULL) {
    std::cout << "t_get_dugit_path: NULL\n";
    return;
  }

  std::cout << "t_get_dugit_path: " << *dugit_path << " => ";
  for (const auto& c : *dugit_path) {
    std::cout << uint32_t(c) << ", ";
  } std::cout << std::endl;
  delete(dugit_path);
}

void t_create_dugit_directory () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_create_dugit_directory: NULL\n";
    return;
  }

  std::string* dugit_path = get_dugit_path(*cwd);
  if (dugit_path != NULL) {
    delete(cwd);
    std::cout << "t_create_dugit_directory: " << *dugit_path << std::endl;
    delete(dugit_path);
    return;
  }

  std::string* toplevel_path = get_toplevel_path_manually(*cwd);
  if (toplevel_path == NULL) {
    delete(cwd);
    std::cout << "t_get_dugit_path: NULL\n";
    return;
  }

  if (create_dugit_directory(*toplevel_path))
    std::cout << "t_create_dugit_directory: " << *toplevel_path << "/.dugit" << std::endl;
  else std::cout << "t_create_dugit_directory: NULL\n";
  delete(cwd);
  delete(toplevel_path);
}

void t_add_dugit_to_gitignore () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_add_dugit_to_gitignore: NULL\n";
    return;
  }

  std::string* toplevel_path = get_toplevel_path_manually(*cwd);
  delete(cwd);
  if (toplevel_path == NULL) {
    std::cout << "t_add_dugit_to_gitignore: NULL\n";
    return;
  }

  if (!check_dugit_in_gitignore(*toplevel_path)) {
    if (add_dugit_to_gitignore(*toplevel_path))
      std::cout << "t_add_dugit_to_gitignore: SUCCESS\n";
    else std::cout << "t_add_dugit_to_gitignore: NULL\n";
  } else std::cout << "t_add_dugit_to_gitignore: SUCCESS\n";
  delete(toplevel_path);
}

void t_check_lock_file () {
  std::string* ppid = get_ppid();
  if (ppid == NULL) {
    std::cout << "t_check_lock_file: NULL\n";
    return;
  }

  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    delete(ppid);
    std::cout << "t_check_lock_file: NULL\n";
    return;
  }

  std::string* dugit_path = get_dugit_path(*cwd);
  if (dugit_path == NULL) {
    delete(ppid);
    delete(cwd);
    std::cout << "t_check_lock_file: NULL\n";
    return;
  }

  if (check_lock_file(*dugit_path + "/.lock", *ppid))
    std::cout << "t_check_lock_file: SUCCESS\n";
  else std::cout << "t_check_lock_file: NULL\n";
  delete(ppid);
  delete(cwd);
  delete(dugit_path);
}

void t_set_lock_file () {
  std::string* ppid = get_ppid();
  if (ppid == NULL) {
    std::cout << "t_set_lock_file: NULL\n";
    return;
  }

  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    delete(ppid);
    std::cout << "t_set_lock_file: NULL\n";
    return;
  }

  std::string* dugit_path = get_dugit_path(*cwd);
  if (dugit_path == NULL) {
    delete(ppid);
    delete(cwd);
    std::cout << "t_set_lock_file: NULL\n";
    return;
  }

  if (set_lock_file(*dugit_path + "/.lock", *ppid))
    std::cout << "t_set_lock_file: SUCCESS\n";
  else std::cout << "t_set_lock_file: NULL\n";
  delete(ppid);
  delete(cwd);
  delete(dugit_path);
}

void t_unset_lock_file () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_unset_lock_file: NULL\n";
    return;
  }

  std::string* dugit_path = get_dugit_path(*cwd);
  if (dugit_path == NULL) {
    delete(cwd);
    std::cout << "t_unset_lock_file: NULL\n";
    return;
  }

  if (unset_lock_file(*dugit_path + "/.lock"))
    std::cout << "t_unset_lock_file: SUCCESS\n";
  else std::cout << "t_unset_lock_file: NULL\n";
  delete(cwd);
  delete(dugit_path);
}

void t_check_dugit_external_dependencies () {
  if (check_dugit_external_dependencies())
    std::cout << "t_check_dugit_external_dependencies: SUCCESS\n";
  else std::cout << "t_check_dugit_external_dependencies: NULL\n";
}

void t_fetch_remote () {
  std::string* cwd = get_cwd();
  if (cwd == NULL) {
    std::cout << "t_fetch_remote: NULL\n";
    return;
  }

  std::string* remote_names = get_remote_names(*cwd);
  if (remote_names == NULL) {
    delete(cwd);
    std::cout << "t_fetch_remote: NULL\n";
    return;
  }

  std::string* current_branch_name = get_current_branch_name(*cwd);
  if (current_branch_name == NULL) {
    delete(cwd);
    delete(remote_names);
    std::cout << "t_fetch_remote: NULL\n";
    return;
  }

  for (const auto& remote_name : get_lines_from_string(*remote_names)) {
    if (fetch_remote(*cwd, remote_name, *current_branch_name))
      std::cout << "t_fetch_remote: " << remote_name << "/" << *current_branch_name << " SUCCESS"<< std::endl;
    else std::cout << "t_fetch_remote: " << remote_name << "/" << *current_branch_name << " NULL"<< std::endl;
  }

  delete(cwd);
  delete(remote_names);
  delete(current_branch_name);
}
