#include "session.h"

Session::Session () {
  // Dummy method
  return;
}

Session::~Session () {
  // Destructor sequence
  this->clean_up();

  // Remove branches and remotes
  for (const auto& branch : this->branches)
    delete(branch);

  for (const auto& remote : this->remotes)
    delete(remote);
}

bool Session::clean_up () {
  // Unset lock
  if (this->lock_secured)
    this->lock_secured = !unset_lock_file(this->dugit_path + "/.lock");

  // Abort merge
  if (check_merge_head_file(this->toplevel_path) ||
  check_merge_msg_file(this->toplevel_path) ||
  check_merge_mode_file(this->toplevel_path)) {
    if (this->flags.at("--abort-merge")) {
      merge_abort(this->toplevel_path);
    } else {
      if (this->stashed_changes) std::cout << "\033[41;1mNot popping stash, deal with merge first, or abort merge via 'git merge --abort'.\nThen pop stash via 'git stash pop' (--index to regain staged/unstaged structure)...\033[0m\n";
      return true;
    }
  }

  // Pop stash
  if (this->stashed_changes) {
    std::cout << "Popping stash..." << std::endl;
    if (!pop_stash(this->toplevel_path, this->flags.at("--keep-index")))
      return false;
    this->stashed_changes = false;
    std::cout << "Popped stash successfully..." << std::endl;
  } return true;
}

// Print dugit version
void print_dugit_version () {
  std::cout << "dugit version " << dugit_version << std::endl;
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
    "    --stage-all     If you wish to stage (add) all changes made to the repository",
    "                    including untracked changes (not yet tracked by git, if you do",
    "                    'git status' git will tell you if you have any untracked changes),",
    "                    you can use this flag to stage untracked changes. Otherwise, by",
    "                    default, Dugit stages only tracked changes.",
    "",
    "    --commit        When using the \"sync\" command, by default, Dugit performs an",
    "                    automatic stash so that any uncommited changes (both staged and",
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
    "    examples        Get examples of usage.",
    "",
    "    version         Use this command to get the currently installed version of dugit.",
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
    "                    \"--commit\" flag. (See examples at the end)",
    "",
    "",
    "To see git specific information see: git help",
  };

  for (const auto& line : help_string)
    std::cout << line << std::endl;
}

// Print examples of usage
void print_usage_examples () {
  std::vector<std::string> help_string = {
    "\033[4;1mExample Usage\033[0m",
    "",
    "To find out the installed version of dugit, one can use the following command.",
    "    dugit version",
    "",
    "If one is stuck and does not know how to use dugit, the same information as displayed",
    "in this README will be printed to one's command-line via this command.",
    "    dugit help",
    "",
    "\033[4;1mCommitting\033[0m",
    "Commit only tracked changes. The user will be asked whether they want to ingnore",
    "the untracked changes.",
    "    dugit commit",
    "",
    "Commit only tracked changes, and avoid the untracked changes warning.",
    "    dugit commit --no-warning",
    "",
    "Commit only tracked changes, avoid the untracked changes warning, and generate an",
    "automatic commit message. This is the quickest way to commit your work.",
    "    dugit commit --no-warning --auto-message",
    "",
    "Commit everything, including untracked changes.",
    "    dugit commit --stage-all",
    "",
    "Commit everything, including untracked changes, with an automatic message.",
    "This is the fastest way to commit all of your changes.",
    "    dugit commit --stage-all --auto-message",
    "",
    "\033[41;1mNotice, using '--no-warning' when '--stage-all' is enabled with the 'commit' command\033[0m",
    "\033[41;1mdoes nothing, as the only warning one may get while committing is to check whether\033[0m",
    "\033[41;1mone indeed wants to not stage the untrackted changes within the repository.\033[0m",
    "",
    "\033[4;1mSyncing\033[0m",
    "The simplest way to sync one's local and remote repositories",
    "(without committing one's un-committed local changes) is to use the `sync` command in its simplest form.",
    "This process stashes any local un-committed changes, fetches changes from each remote repository",
    "connected, attempts to merge these remote changes into the local repository, and commits all of",
    "the merges. After this, the local merge of all remote repositories is pushed back to each remote repository,",
    "and at the end, any un-committed changes that were stashed earlier are popped back.",
    "    dugit sync",
    "",
    "\033[41;1mNotice, any flags used for committing will apply to committing of the merges at the end.\033[0m",
    "\033[41;1mIf using '--auto-message', an automatic merge commit message will be generated.\033[0m",
    "\033[41;1mIf not using '--no-warning' you will be asked about any untracked changes and whether you would like\033[0m",
    "\033[41;1mto stage them. If you use the '--stage-all' flag, **untracked changes will be automatically\033[0m",
    "\033[41;1madded to your merge commit**, this will have to be fixed in later patches.\033[0m",
    "",
    "If the user would like to fast forward merge, this can be enabled simply with the following.",
    "    dugit sync --fast-forward",
    "",
    "If the user would like to abort if any of the merges fail, this can be done using the following command.",
    "Here, the merge will be aborted, and dugit will attempt to pop back any stashed changes.",
    "    dugit sync --abort-merge",
    "",
    "If the user would like to commit local un-committed changes (staged and unstage),",
    "the latter can be achieved via the following command.",
    "    dugit sync --commit",
    "",
    "\033[41;1mNotice, any flags used for committing will apply to both committing of the local changes,\033[0m",
    "\033[41;1mand committing of the merges at the end. If using '--auto-message', an automatic merge commit\033[0m",
    "\033[41;1mmessage will be generated when committing merges. If not using '--no-warning' you will once\033[0m",
    "\033[41;1magain be asked about any untracked changes and whether you would like to stage them.\033[0m",
    "",
    "One can use all of the flags mentioned for committing in the latter section with the 'sync' command",
    "in the same way. Having read the previous sections, one should understand what happens",
    "in each combination of each flag."
    "    dugit sync --commit --stage-all",
    "    dugit sync --commit --stage-all --abort-merge",
    "    dugit sync --commit --stage-all --abort-merge --auto-message",
    "    dugit sync --commit --stage-all --abort-merge --auto-message --no-warning",
    "    dugit sync --commit --stage-all --abort-merge --auto-message --no-warning --fast-forward",
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

    for (const auto& command : this->commands) {
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

  // Check command flags
  for (const auto& arg : args) {
    bool found = false;
    std::string err_msg = '\"' + arg + "\" flag not recognized.\n";
    if (arg[0] == '-' || arg[1] == '-') {
      for (auto& flag : this->flags) {
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
  else if (args.front() == "version")
    print_dugit_version();
  else if (args.front() == "examples")
    print_usage_examples();
  else if (args.front() == "commit") {
    if (!this->commit_repository()) {
      std::string err_msg = "fatal: Could not sync repository at " + this->toplevel_path + "\nCheck your git status for more information: git status";
      perror(err_msg.c_str());
      return false;
    }
  }
  else if (args.front() == "sync") {
    if (!this->sync_repository()) {
      std::string err_msg = "fatal: Could not sync repository at " + this->toplevel_path + "\nCheck your git status for more information: git status";
      perror(err_msg.c_str());
      return false;
    }
  } else
    return false;

  return true;
}

// No override startup sequence
bool Session::session_startup_sequence () {
  return this->session_startup_sequence("");
}

// Startup Sequence
bool Session::session_startup_sequence (const std::string path) {
  // Set lock secured status
  this->lock_secured = false;

  // Set stashed changes status
  this->stashed_changes = false;

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

  // Set Working Path (check path override)
  if (!path.empty()) {
    if (!dir_exists(path))
      return false;
    this->working_path = path;
  } else {
    std::string* working_path = get_cwd();
    if (working_path == NULL)
      return false;
    
    this->working_path = *working_path;
    delete(working_path);
  }

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
    new_branch->remotes = {};
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
          local_branch->remotes.push_back(this->remotes.at(remote));
          found = true;
          break;
        }
      } if (!found) {
        // Create new remote-only branch
        Branch* new_branch = new Branch;
        new_branch->name = remote_branch_name;
        new_branch->is_local = false;
        new_branch->remotes = {this->remotes.at(remote)};
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

bool Session::commit_repository () {
  std::string* diff;

  // Check if any untracked changes
  if (check_untracked(this->toplevel_path)) {
    if (!this->flags.at("--no-warning")) {
      diff = get_status(this->toplevel_path);
      if (diff != NULL) {
        std::cout << std::endl << *diff << std::endl;
        delete(diff);
        this->flags.at("--stage-all") = response_generator("There are untracked changes in your repository.\nWould you like to stage (add) these untracked changes to your next commit?");
      }
    }
  } diff = NULL;

  // Check if any changes to stage
  diff = get_diff_uncached(this->toplevel_path);
  if (diff != NULL) {
    bool diff_empty = diff->empty();
    delete(diff);

    // Stage Changes
    if (!diff_empty) {
      std::cout << "Staging..." << std::endl;
      if (!stage_changes(this->toplevel_path, this->flags.at("--stage-all")))
        return false;
      std::cout << "Staging successful..." << std::endl;
    }
  } diff = NULL;

  // Check if anything to commit
  diff = get_diff_cached(this->toplevel_path);
  if (diff != NULL) {
    bool diff_empty = diff->empty();
    delete(diff);
    
    // Commit staged changes
    if (!diff_empty) {
      std::cout << "Committing..." << std::endl;
      std::string commit_message = "";
      if (this->flags.at("--auto-message"))
        commit_message = commit_local_message(this->toplevel_path);
      else commit_message = commit_custom_message();
      clean_commit_message(commit_message);
      if (!commit(this->toplevel_path, commit_message))
        return false;
      std::cout << "Commit successful..." << std::endl;
    }
  }

  return true;
}

// Stash sequence
bool Session::stash_repository () {
  // Check if in the middle of a merge right now
  std::string* diff[2];
  std::string* status;
  bool diff_found = false;

  diff[0] = get_diff_uncached(this->toplevel_path);
  if (diff[0] == NULL) return false;
  if (!diff[0]->empty()) diff_found = true;
  delete(diff[0]);
  
  diff[1] = get_diff_cached(this->toplevel_path);
  if (diff[1] == NULL) return false;
  if (!diff[1]->empty()) diff_found = true;
  delete(diff[1]);

  status = get_status(this->toplevel_path);
  if (status == NULL) return false;

  if (diff_found) {
    if (check_merge_head_file(this->toplevel_path) ||
    check_merge_msg_file(this->toplevel_path) ||
    check_merge_mode_file(this->toplevel_path)) {
      if (!this->flags.at("--no-warning")) {
        std::cout << std::endl << *status << std::endl;
        this->flags.at("--commit") = response_generator("You are currently inside a merge operation that has yet to be committed.\nWould you like to commit these merge changes?");
      }
    } if (status != NULL) delete(status); status = NULL;

    if (!this->flags.at("--commit")) {
      std::cout << "Stashing..." << std::endl;
      if (!stash(this->toplevel_path, this->flags.at("--keep-index")))
        return false;
      this->stashed_changes = true;
      std::cout << "Stashing successful..." << std::endl;
    }
  } if (status != NULL) delete(status); status = NULL;
  return true;
}

bool Session::sync_repository () {
  /*
    Sync the repository on the current
    branch that is loaded.

    Currently very unsafe, does not
    check for possible merge conflicts.
  */

  std::string* diff;

  // Apply commits if enabled
  if (this->flags.at("--commit")) {
    if (!this->commit_repository())
      return false;
  } else if (!this->stash_repository())
    return false;

  // Fetch and merge from remote repositories that have the currently selected branch
  bool log_diff_found = false;
  for (const auto& remote : this->current_branch->remotes) {
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
      if (!merge(this->toplevel_path, remote->name, this->current_branch->name, this->flags.at("--fast-forward")))
        return false;
    } else {
      delete(log_diff);
      std::cout << "Nothing to merge from " << remote->name << '/' << this->current_branch->name << std::endl;
    }
  }

  // Ask whether to commit these merges
  if (log_diff_found ||
  check_merge_head_file(this->toplevel_path) ||
  check_merge_msg_file(this->toplevel_path) ||
  check_merge_mode_file(this->toplevel_path)) {
    // Print Current Status
    std::string* status = get_status(this->toplevel_path);
    if (status == NULL)
      return false;
    std::cout << "\nCurrent Status:\n" << *status << std::endl;
    delete(status); status = NULL;
    if (!this->flags.at("--no-warning") &&
    !response_generator("\nWould you like to go ahead an commit these merges?"))
      return true;

    // Check if anything to commit
    if (!this->commit_repository())
      return false;
  }

  // Push merged to where necessary
  for (const auto& remote : this->remotes) {
    std::string* log_diff;
    if (!this->current_branch->remotes.empty() &&
    std::find(this->current_branch->remotes.begin(), this->current_branch->remotes.end(), remote) != this->current_branch->remotes.end()) {
      // Do this check only if the branch exists on the remote
      log_diff = get_log_diff(this->toplevel_path, remote->name + '/' + this->current_branch->name, this->current_branch->name);
      if (log_diff == NULL)
        return false;
    } else log_diff = new std::string("branch not in repository");
    
    if (!log_diff->empty()) {
      delete(log_diff);
      std::cout << "Pushing to " << remote->name << '/' << this->current_branch->name << std::endl;
      if (!push_remote(this->toplevel_path, remote->name, this->current_branch->name))
        return false;
    } else {
      delete(log_diff);
      std::cout << "Nothing to push to " << remote->name << '/' << this->current_branch->name << std::endl;
    }
  }

  return true;
}
