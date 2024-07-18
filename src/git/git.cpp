#include "git.h"

// Run command and get output, return NULL if any error
std::string execute_with_output(const std::string& command) {
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) {
    perror("popen");
    return nullstr;
  }

  std::ostringstream oss;
  char buffer[4096];
  while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
    oss << buffer;
  }

  int exitCode = pclose(pipe);
  if (exitCode == -1) {
    perror("pclose");
    return nullstr;
  }

  // Filter out special characters
  std::string oss_str = oss.str();
  std::string output;

  for (const auto& c : oss_str) {
    if (c < 32 && c != 10) continue;
    else output += c;
  }

  return output;
}

std::string execute_with_output(const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& command : commands) {
    concatenated += command + ' ';
  }

  return execute_with_output(concatenated);
}

// Single line out
std::string execute_with_output_single_line (const std::string& command) {
  /*
    A lot of functions expect a single
    line output, for which the extraction
    process looks the same.
  */

  // Get output
  std::string command_out = execute_with_output(command);

  // If we receive a nullstr, return nullstr
  if (command_out == nullstr)
    return nullstr;

  // Delimit string
  std::vector<std::string> lines = get_lines_from_string(command_out);

  if (lines.empty())
    return nullstr;

  return lines.front();
}

std::string execute_with_output_single_line (const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& command : commands) {
    concatenated += command + ' ';
  }

  return execute_with_output_single_line(concatenated);
}

// Multi line out
std::vector<std::string> execute_with_output_multi_line(const std::string& command) {
  std::string command_out = execute_with_output(command);

  // If we receive a nullstr, return an empty vector
  if (command_out == nullstr)
    return std::vector<std::string>();

  return get_lines_from_string(command_out);
}

std::vector<std::string> execute_with_output_multi_line (const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& command : commands) {
    concatenated += command + ' ';
  }

  return execute_with_output_multi_line(concatenated);
}

// Extract lines from string
std::vector<std::string> get_lines_from_string(const std::string& s) {
  std::stringstream ss(s);
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(ss, line, '\n')) {
    lines.push_back(line);
  } return lines;
}

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
  std::string git_version = execute_with_output(command);

  // Extract the version number
  if (git_version == nullstr)
    return nullstr;

  std::vector<std::string> git_version_lines = get_lines_from_string(git_version);
  if (git_version_lines.size() != 1)
    return nullstr;

  std::stringstream ss(git_version_lines.front());
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

// Trim front of a string
bool string_trim_front(std::string& s, const unsigned long long len) {
  if (s.length() < len) {
    perror("string_trim_front(): Trying to trim line shorter than expected.");
    return false;
  }
  
  s.erase(0, len);
  return true;
}

bool strings_trim_fronts(std::vector<std::string>& lines, const unsigned long long len) {
  // Check that all lines are long enough
  for (const auto& line : lines) {
    if (line.length() < len) {
      perror("strings_trim_fronts(): Trying to trim line shorter than expected.");
      return false;
    }
  }

  // Trim characters from the front of each line
  for (auto& line : lines)
    string_trim_front(line, len);

  return true;
}

// Trim rear of a string
bool string_trim_rear(std::string& s, const unsigned long long len) {
  if (s.length() < len) {
    perror("string_trim_rear(): Trying to trim line shorter than expected.");
    return false;
  }
  
  s.erase(s.length() - (len + 1), len);
  return true;
}

bool strings_trim_rears(std::vector<std::string>& lines, const unsigned long long len) {
  // Check that all lines are long enough
  for (const auto& line : lines) {
    if (line.length() < len) {
      perror("strings_trim_rears(): Trying to trim line shorter than expected.");
      return false;
    }
  }

  // Trim characters from the rear of each line
  for (auto& line : lines)
    string_trim_rear(line, len);

  return true;
}