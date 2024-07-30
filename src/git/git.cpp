#include "git.h"

// Get git version
std::string get_git_version() {
  /*
    Either git version or
    git --version works, or
    git -v

    The typical output to
    this is also:
    git version x.xx.x
  */

  std::string command = "git -v";

  // Get the git version
  std::string git_version = execute_with_output_single_line(command);

  // Extract the version number
  if (git_version == nullstr)
    return nullstr;

  std::stringstream ss(git_version);
  std::vector<std::string> words;
  std::string word;
  while (std::getline(ss, word, ' ')) {
    words.push_back(word);
  }

  // Check Syntax
  if (words.size() < 3)
    return nullstr;

  if (words[0] != "git" || words[1] != "version")
    return nullstr;

  return words[2];
}

// Check dugit external dependencies
bool check_dugit_external_dependencies () {
  /*
    Most importantly, one needs
    git to be installed on their
    host machine.
  */

  return check_external_dependencies(std::vector<std::string>({
    "git",
  }));
}

// Get remote names
std::vector<std::string> get_remote_names(const std::string& working_path) {
  /*
    We can use git remote to get all of the
    names of each connected remote, simply
    delimited by new-lines.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&","git", "remote"
  };
  return execute_with_output_multi_line(commands);
}

// Get remote push links, filtered by remote name and direction
std::vector<std::string> get_remote_links(const std::string& working_path, const std::string& remote_name, const std::string& direction) {
  /*
    To get the remote push links, one can
    use git remote get-url remote_name --push or --all,
    we can also verify the direction of each url via
    git remote -v.

    The output is expected to come in the format,
    link

    and for the second command, the output is strange
    with padding characters, hence the first command is
    used to fetch the name, and the format is like so,
    remote_name  link (direction)
    with hidden padding characters between each word.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&","git", "remote", "get-url", remote_name, "--all"
  };
  std::vector<std::string> verify_commands = {
    "cd", working_path, "&&", "git", "remote", "-v"
  };

  // Get remote links
  std::vector<std::string> remote_links = execute_with_output_multi_line(commands);
  std::vector<std::string> verify_remote_links = execute_with_output_multi_line(verify_commands);

  // If we receive a nullstr, return an empty vector
  if (remote_links.empty() || verify_remote_links.empty())
    return std::vector<std::string>();

  std::vector<std::string> output;
  for (const auto& link : remote_links) {
    for (const auto& line : verify_remote_links) {
      if (line.find(link) != std::string::npos &&
      line.find(direction) != std::string::npos &&
      line.find(remote_name) == 0)
        output.push_back(link);
    }
  }

  return output;
}

// Get local branch names
std::vector<std::string> get_local_branch_names (const std::string& working_path) {
  /*
    This can be achieved using git branch, and the
    output is expected to be in the format of
      branch_name, notice the two spaces.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "branch"
  };

  std::vector<std::string> local_branch_names = execute_with_output_multi_line(commands);

  if (local_branch_names.empty() ||
    !strings_trim_fronts(local_branch_names, 2))
    return std::vector<std::string>();

  return local_branch_names;
}

// Get remote branch names, filtered by remote name
std::vector<std::string> get_remote_branch_names (const std::string& working_path, const std::string& remote_name) {
  /*
    Remote branch names can be
    fetched with git branch -r. The
    output has the expected format
    of remote_name/branch_name.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "branch", "-r"
  };

  // Get remote branch names
  std::vector<std::string> command_out = execute_with_output_multi_line(commands);

  if (command_out.empty())
    return std::vector<std::string>();

  // Filter for remote_name
  std::vector<std::string> remote_branch_names;
  for (const auto& name : command_out) {
    // Skip if line contains "HEAD"
    if (name.find("HEAD") != std::string::npos)
      continue;

    // Filter for remote name
    if (name.find(remote_name) == std::string::npos)
      continue;
    
    remote_branch_names.push_back(name);
  }

  if (!strings_trim_fronts(remote_branch_names, remote_name.length() + 3))
    return std::vector<std::string>();

  return remote_branch_names;
}

// Get current branch name
std::string get_current_branch_name (const std::string& working_path) {
  /*
    The current branch name can be
    fetched with the command,
    git branch --show-current.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "branch", "--show-current"
  }; return execute_with_output_single_line(commands);
}

// Get Super Project Working Tree path
std::string get_superproject_working_tree_path (const std::string& working_path) {
  /*
    The command,
    git rev-parse --show-superproject-working-tree,
    can be used to return the superproject working
    tree root path if the current directory you are
    in is a submodule of the root repository.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "rev-parse", "--show-superproject-working-tree"
  }; return execute_with_output_single_line(commands);
}

// Get Top Level path
std::string get_toplevel_path (const std::string& working_path) {
  /*
    To get the top level path
    of the current repository, 
    this includes submodules,
    one can use the command,
    git rev-parse --show-toplevel
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "rev-parse", "--show-toplevel"
  }; return execute_with_output_single_line(commands);  
}

// Get Super Project Working Tree path manually
std::string get_superproject_path_manually (const std::string& working_path) {
  /*
    We can get the toplevel path of the
    super project by recursively going 
    through the directories starting
    from the current working directory.
  */

  std::string current_path = working_path;
  std::string last_valid_path = nullstr;

  while (is_inside_working_tree(current_path) &&
    current_path != last_valid_path) {
    last_valid_path = current_path;
    current_path = execute_with_output_single_line(
      std::vector<std::string>(
        {"cd", current_path, "&&", "cd ..", "&&", "pwd"}
      )
    );
  }

  return last_valid_path;
}

// Get Top Level path manually
std::string get_toplevel_path_manually (const std::string& working_path) {
  /*
    We can get the toplevel path of the
    current repo by recursively going 
    backwards through the directories
    starting from the current working
    directory.
  */

  std::string current_path = working_path;
  std::string previous_path;
  std::string first_valid_path = nullstr;

  while (is_inside_working_tree(current_path) &&
    current_path != previous_path &&
    first_valid_path == nullstr) {
    if (dir_exists(current_path + "/.git"))
      first_valid_path = current_path;
    previous_path = current_path;
    current_path = execute_with_output_single_line(
      std::vector<std::string>(
        {"cd", current_path, "&&", "cd ..", "&&", "pwd"}
      )
    );
  }

  return first_valid_path;
}

// Return if currently inside working tree
bool is_inside_working_tree (const std::string& path) {
  /*
    We can check if we are currently
    inside a working tree by using,
    git rev-parse --is-inside-work-tree
  */

  if (path.empty() || path == nullstr)
    return false;

  std::vector<std::string> commands(
    {"cd", path, "&&", "git rev-parse --is-inside-work-tree"}
  );

  std::string output = execute_with_output_single_line(commands);

  if (output == "true")
    return true;
  else return false;
}

// Get .dugit path
std::string get_dugit_path (const std::string& working_path) {
  /*
    The .dugit folder should be
    found in the super project
    directory.
  */

  std::string superproject_path = get_superproject_path_manually(working_path);

  if (superproject_path == nullstr)
    return nullstr;

  /*
    Check if .dugit exists
    in the superproject path.
  */

  superproject_path += "/.dugit";

  if (dir_exists(superproject_path))
    return superproject_path;
  else return nullstr;
}

// create .dugit
bool create_dugit_directory (const std::string& path) {
  return create_dir(path, ".dugit");
}

// Add .dugit to .gitignore
bool add_dugit_to_gitignore (const std::string& path) {
  return append_line_to_file(path + "/.gitignore", ".dugit/");
}

// Check if .dugit is already in the .gitignore
bool check_dugit_in_gitignore (const std::string& path) {
  if (!file_exists(path))
    return false;

  return line_in_file_exists(path + "/.gitignore", ".dugit/");
}

// Check .lock file
bool check_lock_file (const std::string& path, const std::string& ppid) {
  if (!file_exists(path))
    return false;
  
  if (line_pos_in_file(path, ppid) == UINT32_MAX)
    return false;

  return true;
}

// set .lock file
bool set_lock_file (const std::string& path, const std::string& ppid) {
  if (!lock_file(path))
    return false;
  
  if (!append_line_to_file(path, ppid))
    return false;

  return true;
}

// unset .lock file
bool unset_lock_file (const std::string& path) {
  if (!clear_file(path))
    return false;

  if (!unlock_file(path))
    return false;

  return true;
}

// dugit args parser
bool Session::args_parser (const std::vector<std::string> args) {
  /*
    Here we can choose what happens
    based on the args we get.
  */

  // Print Help Screen on empty input
  if (args.empty()) {
    print_help();
    return false;
  }

  uint32_t command = UINT32_MAX;
  for (uint32_t match = 0; match < dugit_commands.size(); match++) {
    if (dugit_commands.at(match) == args.front()) {
      command = match;
      break;
    }
  }

  switch (command) {
    case 0:
      if (!sync_session())
        perror("Could not sync dugit.\n");
      break;
    
    case 1:
      print_help();
      break;

    default:
      break;
  }

  return true;
}

// Constructor with args
Session::Session (const std::vector<std::string>& args) {
  // Perform Startup Sequence
  if (!session_startup_sequence())
    return;

  // Execute args
  if (args_parser(args) == UINT32_MAX)
    return;
}

Session::~Session () {
  return;
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
  std::vector<std::string> local_branch_names = get_local_branch_names(this->toplevel_path);

  // Create Branches
  for (uint32_t branch = 0; branch < local_branch_names.size(); branch++) {
    Branch* new_branch = new Branch;
    new_branch->name = local_branch_names.at(branch);
    new_branch->is_local = true;
    new_branch->is_remote = false;
    this->branches.push_back(new_branch);
  }

  // Get Remote names
  std::vector<std::string> remote_names = get_remote_names(this->toplevel_path);

  // Create Remotes
  for (uint32_t remote = 0; remote < remote_names.size(); remote++) {
    Remote* new_remote = new Remote;
    new_remote->name = remote_names.at(remote);
    new_remote->fetch_links = get_remote_links(this->toplevel_path, remote_names.at(remote), "(fetch)");
    new_remote->push_links = get_remote_links(this->toplevel_path, remote_names.at(remote), "(push)");
    this->remotes.push_back(new_remote);
  }

  // Update Branches to is_remote or create remote-only Branches
  for (uint32_t remote = 0; remote < this->remotes.size(); remote++) {
    // Get Remote Branch names
    std::vector<std::string> remote_branch_names = get_remote_branch_names(this->toplevel_path, this->remotes.at(remote)->name);

    // Filter pre-existing branches
    for (uint32_t remote_branch = 0; remote_branch < remote_branch_names.size(); remote_branch++) {
      bool found = false;
      for (uint32_t local_branch = 0; local_branch < this->branches.size(); local_branch++) {
        if (remote_branch_names.at(remote_branch) == this->branches.at(local_branch)->name) {
          this->branches.at(local_branch)->is_remote = true;
          found = true;
          break;
        }
      } if (!found) {
        // Create new remote-only branch
        Branch* new_branch = new Branch;
        new_branch->name = remote_branch_names.at(remote_branch);
        new_branch->is_local = false;
        new_branch->is_remote = true;
        this->branches.push_back(new_branch);
      }
    }
  }

  // Get current branch
  std::string current_branch_name = get_current_branch_name(this->toplevel_path);
  bool found = false;
  for (uint32_t branch = 0; branch < this->branches.size(); branch++) {
    if (current_branch_name == this->branches.at(branch)->name) {
      this->current_branch = this->branches.at(branch);
      found = true;
      break;
    }
  }
  
  // If could not find current branch, we have a problem
  if (!found)
    return false;

  return true;
}

// Fetch Sequence
bool fetch_remote (const std::string& working_path, const std::string& remote_name, const std::string& branch_name) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "fetch", remote_name, branch_name
  };

  std::vector<std::string> outputs = execute_with_output_multi_line(commands);

  if (outputs.empty())
    return false;

  return true;
}

// Merge Sequence (No commit nor fast-forward, with autostash enabled)
bool merge_nc_nff_a (const std::string& working_path, const std::string& remote_name, const std::string& branch_name) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "merge", "--no-commit", "--no-ff", "--autostash", remote_name + '/' + branch_name
  };
  std::vector<std::string> outputs = execute_with_output_multi_line(commands);

  if (outputs.empty())
    return false;

  return true;
}

// Push Sequence
bool push_remote (const std::string& working_path, const std::string& remote_name, const std::string& branch_name) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "push", remote_name, branch_name
  };

  std::vector<std::string> outputs = execute_with_output_multi_line(commands);

  if (outputs.empty())
    return false;

  return true;
}

void print_help () {
  std::cout << "h" << std::endl;
}