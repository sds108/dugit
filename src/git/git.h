/*
  Here you may find the declarations
  for different aspects of git control
  and git output parsing.
*/

// git.h
#ifndef GIT_H
#define GIT_H

#include "include.h"

const std::vector<std::string> dugit_commands = {
  "sync",
  "help",
  "version",
};

const std::vector<std::string> single_flags = {
  "h",
  "v",
};

const std::vector<std::string> double_flags = {
  "help",
  "version",
};

typedef struct Session Session;
typedef struct Repository Repository;
typedef struct Remote Remote;
typedef struct Branch Branch;

struct Session {
  /*
    This struct holds all session
    specific information, e.g.
    Git version, or recent repos.
  */

  // Git version, this determines how git is called
  std::string git_version;

  // Current Repository
  Repository* repository;

  // $PPID - Parent Process ID
  std::string ppid;

  // Working Path
  std::string working_path;

  // Constructor Sequences
  Session();
  Session(const std::vector<std::string>& args);

  // Destructor Sequence
  ~Session();

  // Startup Sequence
  bool session_startup_sequence();

  // dugit args parser
  bool args_parser(const std::vector<std::string> args);

  // Sync Session
  bool sync_session();
};

struct Repository {
  /*
    This struct holds all repository
    specific information, e.g.
    repository branches, and
    list of remotes.
  */

  // The local path to the repository
  std::string toplevel_path;

  // Current branch
  Branch* current_branch;

  // List of local Branches
  std::vector<Branch*> branches;

  // List of Remotes
  std::vector<Remote*> remotes;

  // Constructor Sequence
  Repository(const std::string& working_path);

  // Destructor Sequence
  ~Repository();
  
  // Initialize Repository Sequence
  bool initialize(const std::string& working_path);

  // Sync Repository
  bool sync_repository();
};

struct Remote {
  /*
    This struct holds all remote
    specific details, e.g. remote
    name, url/ssh link, whether
    it's the origin remote.
  */

  // Remote name
  std::string name;

  // Remote url/ssh links
  std::vector<std::string> push_links;
  std::vector<std::string> fetch_links;
};

struct Branch {
  /*
    This struct holds all branch
    specific information, e.g.
    branch name, whether it's
    local or remote.
  */

  // Branch name
  std::string name;

  // Is it remote
  bool is_remote;

  // Is it local
  bool is_local;
};

// Get git version
std::string get_git_version();

// Check dugit external dependencies
bool check_dugit_external_dependencies();

// Get remote names
std::vector<std::string> get_remote_names(const std::string& working_path);

// Get remote push and fetch links
std::vector<std::string> get_remote_links(const std::string& working_path, const std::string& remote_name, const std::string& direction);

// Get local branch names
std::vector<std::string> get_local_branch_names(const std::string& working_path);

// Get remote branch names, filtered by remote name
std::vector<std::string> get_remote_branch_names(const std::string& working_path, const std::string& remote_name);

// Get current branch name
std::string get_current_branch_name(const std::string& working_path);

// Get Super Project Working Tree path
std::string get_superproject_working_tree_path(const std::string& working_path);

// Get Top Level path
std::string get_toplevel_path(const std::string& working_path);

// Get Super Project Working Tree path manually
std::string get_superproject_path_manually(const std::string& working_path);

// Get Top Level path manually
std::string get_toplevel_path_manually(const std::string& working_path);

// Return if currently inside working tree
bool is_inside_working_tree(const std::string& path);

// Get .dugit path
std::string get_dugit_path(const std::string& working_path);

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

// Fetch Sequence
bool fetch_remote(const std::string& working_path, const std::string& remote_name, const std::string& branch_name);

// Merge Sequence (No commit nor fast-forward, with autostash enabled)
bool merge_nc_nff_a(const std::string& working_path, const std::string& remote_name, const std::string& branch_name);

// Push Sequence
bool push_remote(const std::string& working_path, const std::string& remote_name, const std::string& branch_name);

// Print help menu
void print_help();

#endif
