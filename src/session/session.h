/*
  This header contains the declarations
  for all of the session classes and
  methods.
*/

// session.h
#ifndef SESSION_H
#define SESSION_H

#include "include.h"
#include "git.h"

typedef struct Session Session;
typedef struct Repository Repository;
typedef struct Remote Remote;
typedef struct Branch Branch;

// Release version
const std::string dugit_version = "0.0.2";

struct Session {
  /*
    This struct holds all session
    specific information, e.g.
    Git version, or recent repos,
    repository branches, and
    list of remotes.
  */

  // Git version, this determines how git is called
  std::string git_version;

  // $PPID - Parent Process ID
  std::string ppid;

  // Working Path
  std::string working_path;

  // The local path to the repository
  std::string toplevel_path;

  // .dugit path
  std::string dugit_path;

  // .lock secured
  bool lock_secured;

  // Commands
  const std::vector<std::string> commands = {
    "help",
    "sync",
    "commit",
    "version",
    "examples",
  };

  // Command flags
  std::unordered_map<std::string, bool> flags = {
    {"--auto-message", false},
    {"--stage-all", false},
    {"--commit", false},
    {"--abort-merge", false},
    {"--no-warning", false},
    {"--fast-forward", false},
    {"--keep-index", false},
  };

  // Stashed changes
  bool stashed_changes;

  // Current branch
  Branch* current_branch;

  // List of local Branches
  std::vector<Branch*> branches;

  // List of Remotes
  std::vector<Remote*> remotes;

  // Constructor Sequences
  Session();

  // Destructor Sequence
  ~Session();

  // Startup Sequence
  bool session_startup_sequence();
  bool session_startup_sequence(const std::string path);

  // dugit args parser
  bool args_parser(const std::vector<std::string>& args);

  // Stash sequence
  bool stash_repository();

  // Commit Repository
  bool commit_repository();

  // Sync Repository
  bool sync_repository();

  // Clean up sequence
  bool clean_up();
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

  // List of Remotes this branch is on
  std::vector<Remote*> remotes;

  // Is it local
  bool is_local;
};

// Print help menu
void print_help();

// Print dugit version
void print_dugit_version();

// Print examples of usage
void print_usage_examples();

#endif // SESSION_H