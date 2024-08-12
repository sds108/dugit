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
  // Unset lock
  if (this->lock_secured)
    unset_lock_file(this->dugit_path + "/.lock");

  // Remove branches and remotes
  for (const auto& branch : this->branches)
    delete(branch);

  for (const auto& remote : this->remotes)
    delete(remote);
}

// Print dugit help
void print_help () {
  std::vector<std::string> help_string = {
    "",
    "usage: dugit <command> [<args>]",
    "",
    "\033[4;1mThese are common Dugit arguments, please use carefully:\033[0m",
    "    --abort-merge   When merging branches, any Dugit command will automatically",
    "                    quit if a merge conflict is detected, and leave the merge",
    "                    conflict for the user to fix. If you want Dugit to",
    "                    automatically abort the merge, use this flag.",
    "",
    "    --commit-local  When using the \"sync\" command, by default, Dugit performs an",
    "                    --autostash so that any uncommited changes (both staged and",
    "                    unstaged) are stashed for the duration of the syncing, and will",
    "                    try to be reapplied (popped) back once the syncing is done.",
    "                    If there is work that the user would like to instead first commit",
    "                    before syncing, the user may use this flag to do so.",
    "",
    "    --auto-message  When committing any work or merge, by default the user will be",
    "                    prompted to enter a custom commit message. This flag can be enabled",
    "                    to skip this step and let Dugit make an automatic commit message.",
    "",
    "    --no-warning    When a Dugit command is executed, Dugit will prompt the user",
    "                    with verification questions, giving the user a chance to stop",
    "                    further execution of the command if desired. To skip these",
    "                    warnings, use this flag.",
    "",
    "    --fast-forward  When merging, by default Dugit does not allow fast forwarding,",
    "                    but if desired, the user may this flag to allow fast forwarding.",
    "",
    "",
    "\033[4;1mThese are common Dugit commands used in various situations:\033[0m",
    "\033[41;1mPlease read how to use flags before using commands\033[0m",
    "",
    "    help            Get information on how to use Dugit (this command).",
    "",
    "    sync    <args>  This command's main purpose sync each of the connected remotes with",
    "                    each other, this entails fetching changes from each remote,",
    "                    merging on the local repository, committing all of the merges,",
    "                    and then pushing back to each remote.",
    "",
    "    commit  <args>  This command's main purpose is to stage and commit all local",
    "                    changes on the local repository. This command does not push or sync",
    "                    with the remote repositories. If you want to automatically push",
    "                    the commit as well, look into using the \"sync\" command with the",
    "                    \"--commit-local\" flag. (See examples at the end)",
    "",
    "",
    "\033[4;1mExamples:\033[0m",
    "",
    "To see git specific information see: git help",
  };

  for (const auto& line : help_string)
    std::cout << line << std::endl;
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

  // Commands
  std::vector<std::string> commands = {
    "help",
    "sync",
    "commit",
  };

  // Check commands
  for (const auto& arg : args) {
    bool found = false;
    std::string err_msg = '\"' + arg + "\" command not recognized.\n";
    if (arg.length() < 2) {
      perror(err_msg.c_str());
      return false;
    }

    if (arg[0] == '-' && arg[1] == '-')
      continue;

    for (const auto& command : commands) {
      if (arg == command) {
        found = true;
        break;
      }
    }

    if (!found) {
      perror(err_msg.c_str());
      return false;
    }
  }

  // Command flags
  std::unordered_map<std::string, bool> flags = {
    {"--auto-message", false},
    {"--commit-local", false},
    {"--abort-merge", false},
    {"--no-warning", false},
    {"--fast-forward", false},
  };

  // Check command flags
  for (const auto& arg : args) {
    bool found = false;
    std::string err_msg = '\"' + arg + "\" flag not recognized.\n";
    if (arg[0] == '-' || arg[1] == '-') {
      for (auto& flag : flags) {
        if (arg == flag.first) {
          flag.second = true;
          found = true;
          break;
        }
      }

      if (!found) {
        perror(err_msg.c_str());
        return false;
      }
    }
  }

  if (args.front() == "help")
    print_help();
  else if (args.front() == "commit") {
    if (!this->commit_repository(flags)) {
      std::string err_msg = "fatal: Could not sync repository at " + this->toplevel_path + "\nCheck your git status for more information: git status";
      perror(err_msg.c_str());
      return false;
    }
  }
  else if (args.front() == "sync") {
    if (!this->sync_repository(flags)) {
      std::string err_msg = "fatal: Could not sync repository at " + this->toplevel_path + "\nCheck your git status for more information: git status";
      perror(err_msg.c_str());
      return false;
    }
  } else
    return false;

  return true;
}

// Startup Sequence
bool Session::session_startup_sequence () {
  // Set lock secured status
  this->lock_secured = false;

  // Check dugit dependencies
  if (!check_dugit_external_dependencies())
    return false;

  // Check git version
  std::string* git_version = get_git_version();
  if (git_version == NULL)
    return false;
  
  this->git_version = *git_version;
  delete(git_version);

  // Get PPID
  std::string* ppid = get_ppid();
  if (ppid == NULL)
    return false;
  
  this->ppid =*ppid;
  delete(ppid);

  // Set Working Path
  std::string* working_path = get_cwd();
  if (working_path == NULL)
    return false;
  
  this->working_path = *working_path;
  delete(working_path);

  // Check if inside git repository
  if (!is_inside_working_tree(this->working_path))
    return false;

    // Fetch Repository toplevel_path
  std::string* toplevel_path = get_toplevel_path_manually(this->working_path);
  if (toplevel_path == NULL)
    return false;

  this->toplevel_path = *toplevel_path;
  delete(toplevel_path);

  std::string* dugit_path = get_dugit_path(this->working_path);
  if (dugit_path == NULL)
    // Create .dugit directory
    if (!create_dugit_directory(this->toplevel_path))
      return false;
    // Try again
    else dugit_path = get_dugit_path(this->working_path);
  
  // If failed this time, quit
  if (dugit_path == NULL)
    return false;

  this->dugit_path = *dugit_path;
  delete(dugit_path);

  // Acquire .dugit lock
  if (!set_lock_file(this->dugit_path + "/.lock", this->ppid))
    return false;

  // Set lock secured status
  this->lock_secured = true;

  // Get Local Branch names
  std::string* local_branch_names_str = get_local_branch_names(this->toplevel_path);
  if (local_branch_names_str == NULL)
    return false;

  // Create Branches
  std::vector<std::string> local_branch_names = get_lines_from_string(*local_branch_names_str);
  for (uint32_t branch = 0; branch < local_branch_names.size(); branch++) {
    Branch* new_branch = new Branch;
    new_branch->name = local_branch_names.at(branch);
    new_branch->is_local = true;
    new_branch->is_remote = false;
    this->branches.push_back(new_branch);
  } delete(local_branch_names_str);

  // Get Remote names
  std::string* remote_names_str = get_remote_names(this->toplevel_path);
  if (remote_names_str == NULL)
    return false;

  // Create Remotes
  for (const auto& remote_name : get_lines_from_string(*remote_names_str)) {
    Remote* new_remote = new Remote;
    new_remote->name = remote_name;

    std::string* fetch_links_str = get_remote_links(this->toplevel_path, remote_name, "(fetch)");
    if (fetch_links_str != NULL) {
      new_remote->fetch_links = get_lines_from_string(*fetch_links_str);
      delete(fetch_links_str);
    }

    std::string* push_links_str = get_remote_links(this->toplevel_path, remote_name, "(push)");
    if (push_links_str != NULL) {
      new_remote->push_links = get_lines_from_string(*push_links_str);
      delete(push_links_str);
    }

    this->remotes.push_back(new_remote);
  } delete(remote_names_str);

  // Update Branches to is_remote or create remote-only Branches
  for (uint32_t remote = 0; remote < this->remotes.size(); remote++) {
    // Get Remote Branch names
    std::string* remote_branch_names_str = get_remote_branch_names(this->toplevel_path, this->remotes.at(remote)->name);
    if (remote_branch_names_str == NULL)
      continue;

    // Filter pre-existing branches
    for (const auto& remote_branch_name : get_lines_from_string(*remote_branch_names_str)) {
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
    } delete(remote_branch_names_str);
  }

  // Get current branch
  std::string* current_branch_name = get_current_branch_name(this->toplevel_path);
  if (current_branch_name == NULL)
    return false;
  
  bool found = false;
  for (const auto& branch : this->branches) {
    if (*current_branch_name == branch->name) {
      this->current_branch = branch;
      found = true;
      break;
    }
  } delete(current_branch_name);
  
  // If could not find current branch, we have a problem
  if (!found)
    return false;

  return true;
}

bool Session::commit_repository (const std::unordered_map<std::string, bool>& flags) {
  std::string* diff;

  // Check if any changes to stage
  diff = get_diff_uncached(this->toplevel_path);
  if (diff != NULL) {
    bool diff_empty = diff->empty();
    delete(diff);

    // Stage Changes
    if (!diff_empty) {
      std::cout << "Staging..." << std::endl;
      if (!stage_changes(this->toplevel_path))
        return false;
      std::cout << "Staging successful..." << std::endl;
    }
  }

  // Check if anything to commit
  diff = get_diff_cached(this->toplevel_path);
  if (diff != NULL) {
    bool diff_empty = diff->empty();
    delete(diff);
    
    // Commit staged changes
    if (!diff_empty) {
      std::cout << "Committing..." << std::endl;
      std::string commit_message = "";
      if (flags.at("--auto-message")) commit_message = commit_local_message(this->toplevel_path);
      else commit_message = commit_custom_message();
      clean_commit_message(commit_message);
      if (!commit(this->toplevel_path, commit_message))
        return false;
      std::cout << "Commit successful..." << std::endl;
    }
  }

  return true;
}

bool Session::sync_repository (const std::unordered_map<std::string, bool>& flags) {
  /*
    Sync the repository on the current
    branch that is loaded.

    Currently very unsafe, does not
    check for possible merge conflicts.
  */

  std::string* diff;

  // Apply commits if enabled
  if (flags.at("--commit-local")) {
    if (!this->commit_repository(flags))
      return false;
  }

  // Fetch and merge
  bool log_diff_found = false;
  for (const auto& remote : this->remotes) {
    std::cout << "Fetching from " << remote->name << '/' << this->current_branch->name << std::endl;
    if (!fetch_remote(this->toplevel_path, remote->name, this->current_branch->name)) 
      continue;

    std::string* log_diff = get_log_diff(this->toplevel_path, this->current_branch->name, remote->name + '/' + this->current_branch->name);
    if (log_diff == NULL)
      return false;
    
    if (!log_diff->empty()) {
      log_diff_found = true;
      std::cout << "Log Difference between HEAD and " << remote->name << '/' << this->current_branch->name << ":\n" << *log_diff << std::endl;
      delete(log_diff);
      std::cout << "Merging " << remote->name << '/' << this->current_branch->name << std::endl;
      if (!merge(this->toplevel_path, remote->name, this->current_branch->name, flags.at("--fast-forward"))) {
        if (flags.at("--abort-merge")) // Abort merge before moving on
          merge_abort(working_path);
        return false;
      }
    } else delete(log_diff);
  }

  // Ask whether to commit these merges
  if (log_diff_found) {
    // Print Current Status
    std::string* status = get_status(this->toplevel_path);
    if (status == NULL)
      return false;
    std::cout << "\nCurrent Status:\n" << *status << std::endl;
    delete(status);

    while (!flags.at("--no-warning")) {
      std::cout << "\nWould you like to go ahead an commit these merges?\nTo view the differences with HEAD, type and submit \"diff\"\nOtherwise, Press Enter to continue to commit\nPress Ctrl+C or Ctrl+D to cancel" << std::endl;
      std::string input;
      std::getline(std::cin, input);
      if (input == "diff") {
        std::string* diff = get_diff_head(this->toplevel_path);
        if (diff != NULL) {
          std::cout << *diff << std::endl;
          delete(diff);
        }
      }

      if (input.empty()) break;
    }

    // Check if anything to commit
    if (!this->commit_repository(flags))
      return false;
  }

  // Push merged
  for (const auto& remote : this->remotes) {
    std::cout << "Pushing to " << remote->name << '/' << this->current_branch->name << std::endl;
    if (!push_remote(this->toplevel_path, remote->name, this->current_branch->name))
      return false;
  }

  return true;
}
