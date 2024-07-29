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
std::vector<std::string> get_remote_names() {
  /*
    We can use git remote to get all of the
    names of each connected remote, simply
    delimited by new-lines.
  */

  std::string command = "git remote";
  return execute_with_output_multi_line(command);
}

// Get remote push links, filtered by remote name and direction
std::vector<std::string> get_remote_links(const std::string& remote_name, const std::string& direction) {
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

  std::string command = "git remote get-url " + remote_name + " --all";
  std::string verify_command = "git remote -v";

  // Get remote links
  std::vector<std::string> remote_links = execute_with_output_multi_line(command);
  std::vector<std::string> verify_remote_links = execute_with_output_multi_line(verify_command);

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
std::vector<std::string> get_local_branch_names () {
  /*
    This can be achieved using git branch, and the
    output is expected to be in the format of
      branch_name, notice the two spaces.
  */

  std::string command = "git branch";

  std::vector<std::string> local_branch_names = execute_with_output_multi_line(command);

  if (local_branch_names.empty() ||
    !strings_trim_fronts(local_branch_names, 2))
    return std::vector<std::string>();

  return local_branch_names;
}

// Get remote branch names, filtered by remote name
std::vector<std::string> get_remote_branch_names (const std::string& remote_name) {
  /*
    Remote branch names can be
    fetched with git branch -r. The
    output has the expected format
    of remote_name/branch_name.
  */

  std::string command = "git branch -r";

  // Get remote branch names
  std::vector<std::string> command_out = execute_with_output_multi_line(command);

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
std::string get_current_branch_name () {
  /*
    The current branch name can be
    fetched with the command,
    git branch --show-current.
  */

  std::string command = "git branch --show-current";
  return execute_with_output_single_line(command);
}

// Get Super Project Working Tree path
std::string get_superproject_working_tree_path () {
  /*
    The command,
    git rev-parse --show-superproject-working-tree,
    can be used to return the superproject working
    tree root path if the current directory you are
    in is a submodule of the root repository.
  */

  std::string command = "git rev-parse --show-superproject-working-tree";
  return execute_with_output_single_line(command);
}

// Get Top Level path
std::string get_toplevel_path () {
  /*
    To get the top level path
    of the current repository, 
    this includes submodules,
    one can use the command,
    git rev-parse --show-toplevel
  */

  std::string command = "git rev-parse --show-toplevel";
  return execute_with_output_single_line(command);  
}

// Get Super Project Working Tree path manually
std::string get_superproject_path_manually () {
  /*
    We can get the toplevel path of the
    super project by recursively going 
    through the directories starting
    from the current working directory.
  */

  std::string current_path = get_cwd();
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
std::string get_toplevel_path_manually () {
  /*
    We can get the toplevel path of the
    current repo by recursively going 
    backwards through the directories
    starting from the current working
    directory.
  */

  std::string current_path = get_cwd();
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

  std::vector<std::string> commands(
    {"cd", path, "&&", "git rev-parse --is-inside-work-tree"}
  );

  std::string output = execute_with_output_single_line(commands);

  if (output == "true")
    return true;
  else return false;
}

// Get .dugit path
std::string get_dugit_path () {
  /*
    The .dugit folder should be
    found in the super project
    directory.
  */

  std::string superproject_path = get_superproject_path_manually();

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
