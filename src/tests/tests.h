// All unit tests are declared here

#ifndef TESTS_H
#define TESTS_H

#include "include.h"
#include "git.h"
#include "session.h"

// Test runner
void run_tests();

// Unit tests
void t_get_shell();
void t_get_ppid();
void t_get_cwd();
void t_get_exec_path();
void t_get_git_version();
void t_get_remote_names();
void t_get_remote_links();
void t_get_local_branch_names();
void t_get_remote_branch_names();
void t_get_current_branch_name();
void t_get_superproject_working_tree_path();
void t_get_toplevel_path();
void t_get_superproject_path_manually();
void t_get_toplevel_path_manually();
void t_get_dugit_path();
void t_create_dugit_directory();
void t_add_dugit_to_gitignore();
void t_check_lock_file();
void t_set_lock_file();
void t_unset_lock_file();
void t_check_dugit_external_dependencies();
void t_fetch_remote();

#endif