/*
  Here you may find the declarations
  for different aspects of git control
  and git output parsing.
*/

// git.h
#ifndef GIT_H
#define GIT_H

#include "include.h"

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

  // List of recent repositories
  std::vector<Repository*> repositories;

  // Constructor Sequence
  Session();

  // Destructor Sequence
  ~Session();
};

struct Repository {
  /*
    This struct holds all repository
    specific information, e.g.
    repository branches, and
    list of remotes.
  */

  // The local path to the repository
  std::string path;

  // Latest sync
  std::time_t last_sync;

  // Is this a submodule?
  bool submodule;

  // Submodules
  std::vector<Repository*> submodules;

  // List of local Branches
  std::vector<Branch*> branches;

  // List of Remotes
  std::vector<Remote*> remotes;

  // Constructor Sequence
  Repository();

  // Destructor Sequence
  ~Repository();
  
  // Initialize Repository Sequence
  bool initialize();
};

struct Remote {
  /*
    This struct holds all remote
    specific details, e.g. remote
    name, url/ssh link, latest 
    sync time, whether it's the
    origin remote.
  */

  // Remote name
  std::string name;

  // Remote url/ssh links
  std::vector<std::string> push_links;
  std::vector<std::string> fetch_links;

  // Latest sync
  std::time_t last_sync;

  // Is origin?
  bool origin;

  // Constructor Sequence
  Remote();

  // Destructor Sequence
  ~Remote();
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

  // Is it local? remote? or both?
  short location;

  // Constructor Sequence
  Branch();

  // Destructor Sequence
  ~Branch();
};

// Main controller function
void parse_input(const std::string& input);

// Get git version
std::string get_git_version();

// Get remote names
std::vector<std::string> get_remote_names();

// Get remote push and fetch links
std::vector<std::string> get_remote_links(const std::string& remote_name, const std::string& direction);

// Get local branch names
std::vector<std::string> get_local_branch_names();

// Get remote branch names, filtered by remote name
std::vector<std::string> get_remote_branch_names(const std::string& remote_name);

// Get current branch name
std::string get_current_branch_name();

// Get Super Project Working Tree path
std::string get_superproject_working_tree_path();

// Get Top Level path
std::string get_toplevel_path();

// Get Top Level path manually
std::string get_superproject_path_manually();

// Return if currently inside working tree
bool is_inside_working_tree(const std::string& path);

// Get .dugit path
std::string get_dugit_path();

// create .dugit
bool create_dugit_directory(const std::string& path);

// Add .dugit to .gitignore
bool add_dugit_to_gitignore(const std::string& path);

// Check if .dugit is already in the .gitignore
bool check_dugit_in_gitignore(const std::string& path);

#endif
