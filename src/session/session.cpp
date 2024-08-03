#include "session.h"

// Constructor with args
Session::Session (const std::vector<std::string>& args) {
  // Perform Startup Sequence
  if (!session_startup_sequence())
    return;

  // Execute args
  if (args_parser(args) == false)
    return;
}

Session::~Session () {
  return;
}

// Print dugit help
void print_help () {
  std::string help_string = "usage: dugit [-v | --version] [-h | --help]\n             <command> [<args>]\n\nThese are common Dugit commands used in various situations:\n\nSync all of the remotes of a repository (see also: git add remote)\n   sync\n\nTo see git specific information see: git help";
  std::cout << help_string << std::endl;
}

// dugit args parser
bool Session::args_parser (const std::vector<std::string>& args) {
  /*
    Here we can choose what happens
    based on the args we get.
  */

  // Print Help Screen on empty input
  if (args.empty()) {
    print_help();
    return true;
  }

  if (args.front() == "help")
    print_help();
  else if (args.front() == "sync")
    sync_session();

  return true;
}

// Startup Sequence
bool Session::session_startup_sequence () {
  // Check dugit dependencies
  if (!check_dugit_external_dependencies())
    return false;

  // Check git version
  this->git_version = get_git_version();

  // Get PPID
  this->ppid = get_ppid();

  // Set Working Path
  this->working_path = get_cwd();

  // Check if inside git repository
  if (!is_inside_working_tree(this->working_path))
    return false;

  // Build Repository Structure
  this->repository = new Repository(this->working_path);

  return true;
}

// Sync Session
bool Session::sync_session () {
  /*
    Sync the selected repository
    on the current branch that is
    loaded.
  */

  if (!repository->sync_repository()) {
    std::string err_msg = "fatal: Could not sync repository at " + repository->toplevel_path + "\n";
    perror(err_msg.c_str());
    return false;
  } return true;
}

Repository::Repository (const std::string& working_path) {
  // Perform Startup Sequence
  if (!initialize(working_path))
    return;
}

bool Repository::sync_repository () {
  /*
    Sync the repository on the current
    branch that is loaded.

    Currently very unsafe, does not
    check for possible merge conflicts.
  */

  // Fetch and merge
  for (const auto& remote : this->remotes) {
    if (!fetch_remote(this->toplevel_path, remote->name, this->current_branch->name)) {
      std::string err_msg = "Could not fetch from remote: " + remote->name + ", fetch link: " + remote->fetch_links.front() + '\n';
      perror(err_msg.c_str());
      continue;
    }

    if (!merge_nc_nff_a(this->toplevel_path, remote->name, this->current_branch->name)) {
      std::string err_msg = "Could not merge branch: " + remote->name + '/' + this->current_branch->name + '\n';
      perror(err_msg.c_str());
      return false;
    }
  }

  // Print Current Status
  std::cout << get_status(this->toplevel_path) << std::endl;

  // Push merged
  for (const auto& remote : this->remotes) {
    if (!push_remote(this->toplevel_path, remote->name, this->current_branch->name)) {
      std::string err_msg = "Could not push to remote: " + remote->name + ", push link: " + remote->push_links.front() + '\n';
      perror(err_msg.c_str());
      return false;
    }
  }

  return true;
}

// Initialize Repository Sequence
bool Repository::initialize (const std::string& working_path) {
  /*
    Here we need to initialize
    all of the details regarding
    this repository.
  */

  // Fetch Repository toplevel_path
  this->toplevel_path = get_toplevel_path_manually(working_path);
  if (this->toplevel_path == nullstr)
    return false;

  // Get Local Branch names
  std::string local_branch_names_str = get_local_branch_names(this->toplevel_path);
  if (local_branch_names_str == nullstr)
    return false;

  // Create Branches
  std::vector<std::string> local_branch_names = get_lines_from_string(local_branch_names_str);
  for (uint32_t branch = 0; branch < local_branch_names.size(); branch++) {
    Branch* new_branch = new Branch;
    new_branch->name = local_branch_names.at(branch);
    new_branch->is_local = true;
    new_branch->is_remote = false;
    this->branches.push_back(new_branch);
  }

  // Get Remote names
  std::string remote_names_str = get_remote_names(this->toplevel_path);
  if (remote_names_str == nullstr)
    return false;

  // Create Remotes
  for (const auto& remote_name : get_lines_from_string(remote_names_str)) {
    Remote* new_remote = new Remote;
    new_remote->name = remote_name;
    std::string fetch_links_str = get_remote_links(this->toplevel_path, remote_name, "(fetch)");
    if (fetch_links_str != nullstr)
      new_remote->fetch_links = get_lines_from_string(fetch_links_str);
    std::string push_links_str = get_remote_links(this->toplevel_path, remote_name, "(push)");
    if (push_links_str != nullstr)
      new_remote->push_links = get_lines_from_string(push_links_str);
    this->remotes.push_back(new_remote);
  }

  // Update Branches to is_remote or create remote-only Branches
  for (uint32_t remote = 0; remote < this->remotes.size(); remote++) {
    // Get Remote Branch names
    std::string remote_branch_names_str = get_remote_branch_names(this->toplevel_path, this->remotes.at(remote)->name);
    if (remote_branch_names_str == nullstr)
      continue;

    // Filter pre-existing branches
    for (const auto& remote_branch_name : get_lines_from_string(remote_branch_names_str)) {
      bool found = false;
      for (const auto& local_branch : this->branches) {
        if (remote_branch_name == local_branch->name) {
          local_branch->is_remote = true;
          found = true;
          break;
        }
      } if (!found) {
        // Create new remote-only branch
        Branch* new_branch = new Branch;
        new_branch->name = remote_branch_name;
        new_branch->is_local = false;
        new_branch->is_remote = true;
        this->branches.push_back(new_branch);
      }
    }
  }

  // Get current branch
  std::string current_branch_name = get_current_branch_name(this->toplevel_path);
  bool found = false;
  for (const auto& branch : this->branches) {
    if (current_branch_name == branch->name) {
      this->current_branch = branch;
      found = true;
      break;
    }
  }
  
  // If could not find current branch, we have a problem
  if (!found)
    return false;

  return true;
}

