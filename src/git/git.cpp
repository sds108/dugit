#include "git.h"

// Get git version
std::string* get_git_version() {
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
  std::string* git_version = execute_with_output_single_line(command);

  // Extract the version number
  if (git_version == NULL)
    return NULL;

  std::stringstream ss(*git_version);
  std::vector<std::string> words;
  std::string word;
  while (std::getline(ss, word, ' ')) {
    words.push_back(word);
  } delete(git_version);

  // Check Syntax
  if (words.size() < 3)
    return NULL;

  if (words[0] != "git" || words[1] != "version")
    return NULL;

  return new std::string(words[2]);
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
std::string* get_remote_names(const std::string& working_path) {
  /*
    We can use git remote to get all of the
    names of each connected remote, simply
    delimited by new-lines.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&","git", "remote"
  }; return execute_with_output(commands);
}

// Get remote push links, filtered by remote name and direction
std::string* get_remote_links(const std::string& working_path, const std::string& remote_name, const std::string& direction) {
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
  std::string* remote_links_str = execute_with_output(commands);
  if (remote_links_str == NULL)
    return NULL;
  
  std::string* verify_remote_links_str = execute_with_output(verify_commands);
  if (verify_remote_links_str == NULL) {
    delete(remote_links_str);
    return NULL;
  }

  std::vector<std::string> output;
  for (const auto& link : get_lines_from_string(*remote_links_str)) {
    for (const auto& line : get_lines_from_string(*verify_remote_links_str)) {
      if (line.empty() || link.empty()) continue;
      if (line.find(link) != std::string::npos &&
      line.find(direction) != std::string::npos &&
      line.find(remote_name) == 0)
        output.push_back(link);
    }
  }

  delete(remote_links_str);
  delete(verify_remote_links_str);
  return new std::string(get_string_from_lines(output));
}

// Get local branch names
std::string* get_local_branch_names (const std::string& working_path) {
  /*
    This can be achieved using git branch, and the
    output is expected to be in the format of
      branch_name, notice the two spaces.
  */

  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "branch"
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL)
    return NULL;

  std::vector<std::string> local_branch_names = get_lines_from_string(*command_out);
  delete(command_out);
  if (!strings_trim_fronts(local_branch_names, 2))
    return NULL;

  return new std::string(get_string_from_lines(local_branch_names));
}

// Get remote branch names, filtered by remote name
std::string* get_remote_branch_names (const std::string& working_path, const std::string& remote_name) {
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
  std::string* command_out = execute_with_output(commands);

  if (command_out == NULL)
    return NULL;

  // Filter for remote_name
  std::vector<std::string> all_branch_names = get_lines_from_string(*command_out);
  delete(command_out);
  std::vector<std::string> remote_branch_names;
  for (const auto& name : all_branch_names) {
    if (name.empty())
      continue;
    
    // Skip if line contains "HEAD"
    if (name.find("HEAD") != std::string::npos)
      continue;

    // Filter for remote name
    if (name.find(remote_name) == std::string::npos)
      continue;
    
    remote_branch_names.push_back(name);
  }

  if (!strings_trim_fronts(remote_branch_names, remote_name.length() + 3))
    return NULL;
  
  return new std::string(get_string_from_lines(remote_branch_names));
}

// Get current branch name
std::string* get_current_branch_name (const std::string& working_path) {
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
std::string* get_superproject_working_tree_path (const std::string& working_path) {
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
std::string* get_toplevel_path (const std::string& working_path) {
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
std::string* get_superproject_path_manually (const std::string& working_path) {
  /*
    We can get the toplevel path of the
    super project by recursively going 
    through the directories starting
    from the current working directory.
  */

  std::string current_path = working_path;
  std::string last_valid_path = "";

  while (is_inside_working_tree(current_path) &&
    current_path != last_valid_path) {
    last_valid_path = current_path;
    std::string* command_out = execute_with_output_single_line(
      {"cd", current_path, "&&", "cd ..", "&&", "pwd"}
    );
    if (command_out == NULL) return NULL;
    current_path = *command_out;
    delete(command_out);
  }

  if (last_valid_path.empty()) return NULL;
  return new std::string(last_valid_path);
}

// Get Top Level path manually
std::string* get_toplevel_path_manually (const std::string& working_path) {
  /*
    We can get the toplevel path of the
    current repo by recursively going 
    backwards through the directories
    starting from the current working
    directory.
  */

  std::string current_path = working_path;
  std::string previous_path;
  std::string first_valid_path = "";

  while (is_inside_working_tree(current_path) &&
    current_path != previous_path &&
    first_valid_path.empty()) {
    if (dir_exists(current_path + "/.git"))
      first_valid_path = current_path;
    previous_path = current_path;
    std::string* command_out = execute_with_output_single_line(
      {"cd", current_path, "&&", "cd ..", "&&", "pwd"}
    );
    if (command_out == NULL) return NULL;
    current_path = *command_out;
    delete(command_out);
  }

  if (first_valid_path.empty()) return NULL;
  return new std::string(first_valid_path);
}

// Return if currently inside working tree
bool is_inside_working_tree (const std::string& path) {
  /*
    We can check if we are currently
    inside a working tree by using,
    git rev-parse --is-inside-work-tree
  */

  if (path.empty())
    return false;

  std::vector<std::string> commands(
    {"cd", path, "&&", "git rev-parse --is-inside-work-tree"}
  );

  std::string* command_out = execute_with_output_single_line(commands);
  if (command_out == NULL) return NULL;
  std::string result = *command_out;
  delete(command_out);
  if (result == "true")
    return true;
  return false;
}

// Get .dugit path
std::string* get_dugit_path (const std::string& working_path) {
  /*
    The .dugit folder should be
    found in the super project
    directory.
  */

  std::string* superproject_path = get_superproject_path_manually(working_path);

  if (superproject_path == NULL)
    return NULL;

  /*
    Check if .dugit exists
    in the superproject path.
  */

  *superproject_path += "/.dugit";

  if (dir_exists(*superproject_path))
    return superproject_path;
  delete(superproject_path);
  return NULL;
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
  
  if (line_pos_in_file(path, ppid) == -1)
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

// Stage Changes
bool stage_changes (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "add", "."
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "stage_changes() ==> Could not stage (add) changes at path: " + working_path + '\n';
    perror(err_msg.c_str());

    // Unstage everything before moving on
    unstage_changes(working_path);
    return false;
  }

  delete(command_out);
  return true;
}

// Unstage Changes
bool unstage_changes (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "reset"
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "unstage_changes() ==> Could not unstage (remove) staged changes at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return false;
  }

  delete(command_out);
  return true;
}

// Commit Changes
bool commit_changes (const std::string& working_path) {
  std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm* now_tm = std::localtime(&now_time);
  std::ostringstream oss;
  oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
  std::string* diff_file_names_output = get_diff_cached_names(working_path);
  std::string diff_file_names;
  if (diff_file_names_output == NULL)
    return false;
  
  diff_file_names += ", Changes apply to: ";
  for (const auto& name : get_lines_from_string(*diff_file_names_output)) {
    diff_file_names += name + ", ";
    if (diff_file_names.length() > 256) {
      diff_file_names.erase(diff_file_names.begin() + 256, diff_file_names.end());
      diff_file_names += "...";
      break;
    }
  } delete(diff_file_names_output);

  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "commit", "-m", "\"[" + oss.str() + "] Dugit Commit" + diff_file_names + '\"'
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "commit_changes() ==> Could not commit merge at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return false;
  }

  delete(command_out);
  return true;
}

// Fetch Sequence
bool fetch_remote (const std::string& working_path, const std::string& remote_name, const std::string& branch_name) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "fetch", remote_name, branch_name
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "fetch_remote() ==> Could not fetch from remote " + remote_name + '/' + branch_name + '\n';
    perror(err_msg.c_str());
    return false;
  }

  delete(command_out);
  return true;
}

// Merge Sequence (No commit nor fast-forward, with autostash enabled)
bool merge_nc_nff_a (const std::string& working_path, const std::string& remote_name, const std::string& branch_name) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "merge", "--no-commit", "--no-ff", "--autostash", remote_name + '/' + branch_name
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "merge_nc_nff_a() ==> Could not merge with " + remote_name + '/' + branch_name + '\n';
    perror(err_msg.c_str());

    // Abort merge before moving on
    merge_abort(working_path);
    return false;
  }

  delete(command_out);
  return true;
}

// Abort merge
bool merge_abort (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "merge", "--abort"
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "merge_abort() ==> Could not abort merge, please investigate.\n";
    perror(err_msg.c_str());
    return false;
  }

  delete(command_out);
  return true;
}

// Push Sequence
bool push_remote (const std::string& working_path, const std::string& remote_name, const std::string& branch_name) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "push", remote_name, branch_name
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "push_remote() ==> Could not push to " + remote_name + '/' + branch_name + '\n';
    perror(err_msg.c_str());
    return false;
  }

  delete(command_out);
  return true;
}

// Git Status
std::string* get_status (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "status"
  };
  
  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "get_status() ==> Could not get git status at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return NULL;
  } return command_out;
}

// Git Diff with HEAD
std::string* get_diff_head (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "diff", "HEAD"
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "get_diff_head() ==> Could not get git diff HEAD at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return NULL;
  } return command_out;
}

// Git Diff Cached
std::string* get_diff_cached (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "diff", "--cached"
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "get_diff_cached() ==> Could not get git diff --cached at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return NULL;
  } return command_out;
}

// Git Diff cached file names
std::string* get_diff_cached_names (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "diff", "--cached", "--name-only"
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "get_diff_cached_names() ==> Could not get git diff --cached --name-only at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return NULL;
  } return command_out;
}

// Git Diff Uncached
std::string* get_diff_uncached (const std::string& working_path) {
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "diff"
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "get_diff_uncached() ==> Could not get git diff at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return NULL;
  } return command_out;
}

// Git Commit
bool commit_merge (const std::string& working_path) {
  std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::tm* now_tm = std::localtime(&now_time);
  std::ostringstream oss;
  oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
  std::vector<std::string> commands = {
    "cd", working_path, "&&", "git", "commit", "-m", "\"[" + oss.str() + "] Dugit Repository Sync.\""
  };

  std::string* command_out = execute_with_output(commands);
  if (command_out == NULL) {
    std::string err_msg = "commit_merge() ==> Could not commit merge at path: " + working_path + '\n';
    perror(err_msg.c_str());
    return false;
  } delete(command_out);
  return true;
}