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

  // Current branch
  Branch* current_branch;

  // List of local Branches
  std::vector<Branch*> branches;

  // List of Remotes
  std::vector<Remote*> remotes;

  // Constructor Sequences
  Session();
  Session(const std::vector<std::string>& args);

  // Destructor Sequence
  ~Session();

  // Startup Sequence
  bool session_startup_sequence();

  // dugit args parser
  bool args_parser(const std::vector<std::string>& args);

  // Initialize Repository Sequence
  bool initialize(const std::string& working_path);

  // Commit Repository
  bool commit_repository(const std::unordered_map<std::string, bool>& flags);

  // Sync Repository
  bool sync_repository(const std::unordered_map<std::string, bool>& flags);
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

// Print help menu
void print_help();

#endif // SESSION_H