/*
  Here you may find the declarations
  for different aspects of git control
  and git output parsing.
*/

// git.h
#ifndef GIT_H
#define GIT_H

#include "include.h"

// Get git version
std::string* get_git_version();

// Check dugit external dependencies
bool check_dugit_external_dependencies();

// Get remote names
std::string* get_remote_names(const std::string& working_path);

// Get remote push and fetch links
std::string* get_remote_links(const std::string& working_path, const std::string& remote_name, const std::string& direction);

// Get local branch names
std::string* get_local_branch_names(const std::string& working_path);

// Get remote branch names, filtered by remote name
std::string* get_remote_branch_names(const std::string& working_path, const std::string& remote_name);

// Get current branch name
std::string* get_current_branch_name(const std::string& working_path);

// Get Super Project Working Tree path
std::string* get_superproject_working_tree_path(const std::string& working_path);

// Get Top Level path
std::string* get_toplevel_path(const std::string& working_path);

// Get Super Project Working Tree path manually
std::string* get_superproject_path_manually(const std::string& working_path);

// Get Top Level path manually
std::string* get_toplevel_path_manually(const std::string& working_path);

// Return if currently inside working tree
bool is_inside_working_tree(const std::string& path);

// Get .dugit path
std::string* get_dugit_path(const std::string& working_path);

// create .dugit
bool create_dugit_directory(const std::string& path);

// Add .dugit to .gitignore
bool add_dugit_to_gitignore(const std::string& path);

// Check if .dugit is already in the .gitignore
bool check_dugit_in_gitignore(const std::string& path);

// Check .lock file
bool check_lock_file(const std::string& path, const std::string& ppid);

// Set .lock file
bool set_lock_file(const std::string& path, const std::string& ppid);

// Unset .lock file
bool unset_lock_file(const std::string& path);

// Stage Changes
bool stage_changes(const std::string& working_path);

// Unstage Changes
bool unstage_changes(const std::string& working_path);

// Fetch Sequence
bool fetch_remote(const std::string& working_path, const std::string& remote_name, const std::string& branch_name);

// Merge Sequence (No commit nor fast-forward, with autostash enabled)
bool merge(const std::string& working_path, const std::string& remote_name, const std::string& branch_name, const bool ff);

// Abort merge
bool merge_abort(const std::string& working_path);

// Git Commit
bool commit(const std::string& working_path, const std::string& message);

// Push Sequence
bool push_remote(const std::string& working_path, const std::string& remote_name, const std::string& branch_name);

// Git Status
std::string* get_status(const std::string& working_path);

// Git Diff Head
std::string* get_diff_head(const std::string& working_path);

// Git Diff with HEAD and Remote Branch
std::string* get_diff_head_remote(const std::string& working_path, const std::string& remote_branch);

// Git Diff cached
std::string* get_diff_cached(const std::string& working_path);

// Git Diff cached file names
std::string* get_diff_cached_names(const std::string& working_path);

// Git Diff Uncached
std::string* get_diff_uncached(const std::string& working_path);

// Git log diff between local and remote
std::string* get_log_diff(const std::string& working_path, const std::string& branch_a, const std::string branch_b);

// Automatic Commit Message after committing sync merging
std::string commit_sync_message();

// Auto Commit Message
std::string commit_local_message(const std::string& working_path);

// Custom commit message
std::string commit_custom_message();

// Clean commit message
bool clean_commit_message(std::string& message);

#endif
