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

  std::string output = oss.str();

  // Check for end-line at the end of the string
  if (output.back() == '\n') output.pop_back();

  return output;
}

std::string execute_with_output(const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::ostringstream oss;

  for (const auto& command : commands) {
    oss << command << ' ';
  }

  std::string concatenated = oss.str();

  return execute_with_output(concatenated);
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

// Get remote names
std::vector<std::string> get_remote_names() {
  /*
    We can use git remote to get all of the
    names of each connected remote, simply
    delimited by new-lines.
  */

  std::string command = "git remote";

  // Get remote names
  std::string remote_names = execute_with_output(command);

  // If we receive a nullstr, return an empty vector
  if (remote_names == nullstr)
    return std::vector<std::string>();

  // Delimit string
  std::stringstream ss(remote_names);

  std::vector<std::string> names;
  std::string name;
  while (std::getline(ss, name, '\n')) {
    names.push_back(name);
  }

  return names;
}

// Get remote push and fetch links
std::vector<std::string> get_remote_push_links();
std::vector<std::string> get_remote_fetch_links();

// Get local branch names
std::vector<std::string> get_local_branch_names();

// Get remote branch names
std::vector<std::string> get_remote_branch_names();