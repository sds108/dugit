// All unit tests are declared here
#include "include.h"
#include "git.h"

// Test runner
void run_tests();

// Unit tests
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
void t_get_dugit_path();
void t_create_dugit_directory();
void t_add_dugit_to_gitignore();