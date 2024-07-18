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

// Run command and get output
std::string execute_with_output(const std::string& command);
std::string execute_with_output(const std::vector<std::string>& commands);

// Single line out
std::string execute_with_output_single_line(const std::string& command);
std::string execute_with_output_single_line(const std::vector<std::string>& commands);

// Multi line out
std::vector<std::string> execute_with_output_multi_line(const std::string& command);
std::vector<std::string> execute_with_output_multi_line(const std::vector<std::string>& commands);

// Get lines from string
std::vector<std::string> get_lines_from_string(const std::string& s);

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

// Trim front of a string
bool string_trim_front(std::string& s, const unsigned long long len);
bool strings_trim_fronts(std::vector<std::string>& lines, const unsigned long long len);

// Trim rear of a string
bool string_trim_rear(std::string& s, const unsigned long long len);
bool strings_trim_rears(std::vector<std::string>& lines, const unsigned long long len);

#endif