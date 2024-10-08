#include "include.h"

char* get_shell () {
  return getenv("SHELL"); // envariables do not have to be freed
}

std::string* get_ppid () {
  /*
    To prevent race conditions
    between different terminal
    windows using dugit, the
    terminal window ppid can
    be used to decide who owns
    the .lock file.
  */

  std::string command = "echo $PPID";
  return execute_with_output_single_line(command);
}

std::string* get_cwd () {
  char buffer[8192];
  if (getcwd(buffer, sizeof(buffer)) == NULL)
    return NULL;
  return new std::string(buffer);
}

std::string* get_pwd () {
  std::string command = "pwd";
  return execute_with_output_single_line(command);
}

int32_t execute_without_output (const std::string& command) {
  char* shell = get_shell(); // envariables do not have to be freed
  if (shell == NULL)
    return 1;

  int stderr_pipe[2];
  if (pipe(stderr_pipe) != 0) {
    perror("pipe");
    return 1;
  }

  // Fork process into child and parent
  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  } else if (pid == 0) {
    // Child process
    close(stderr_pipe[0]);
    dup2(stderr_pipe[1], STDERR_FILENO);
    close(stderr_pipe[1]);

    execl(shell, shell, "-c", command.c_str(), (char *) NULL);
    _exit(EXIT_FAILURE);
  } else {
    // Parent process
    close(stderr_pipe[1]);

    std::ostringstream stderr_stream;
    char buffer[4096];
    ssize_t count;

    while ((count = read(stderr_pipe[0], buffer, sizeof(buffer))) > 0)
      stderr_stream.write(buffer, count);

    close(stderr_pipe[0]);

    int status;
    waitpid(pid, &status, 0);
    int exit_status = WEXITSTATUS(status);

    if (exit_status != 0) {
      std::string err_msg = "\nCOMMAND: " + command + "\nERROR: " + stderr_stream.str() + '\n';
      // perror(err_msg.c_str());
      return 1;
    }

    return 0;
  }
}

int32_t execute_without_output (const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& command : commands) {
    concatenated += command + ' ';
  }

  return execute_without_output(concatenated);
}

std::string* execute_with_output (const std::string& command) {
  char* shell = get_shell(); // envariables do not have to be freed
  if (shell == NULL)
    return NULL;
  
  int stdout_pipe[2];
  int stderr_pipe[2];
  if (pipe(stdout_pipe) != 0 || pipe(stderr_pipe) != 0) {
    perror("pipe");
    return NULL;
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return NULL;
  } else if (pid == 0) {
    // Child process
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);
    dup2(stdout_pipe[1], STDOUT_FILENO);
    dup2(stderr_pipe[1], STDERR_FILENO);
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    execl(shell, shell, "-c", command.c_str(), (char *) NULL);
    _exit(EXIT_FAILURE);
  } else {
    // Parent process
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    std::ostringstream stdout_stream;
    std::ostringstream stderr_stream;
    char buffer[4096];
    ssize_t count;

    while ((count = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0)
      stdout_stream.write(buffer, count);
    while ((count = read(stderr_pipe[0], buffer, sizeof(buffer))) > 0)
      stderr_stream.write(buffer, count);

    close(stdout_pipe[0]);
    close(stderr_pipe[0]);

    int status;
    waitpid(pid, &status, 0);
    int exit_status = WEXITSTATUS(status);

    if (exit_status != 0) {
      std::string err_msg = "\nCOMMAND: " + command + "\nERROR: " + stderr_stream.str() + "\nOUTPUT: " + stdout_stream.str() + '\n';
      // perror(err_msg.c_str());
      return NULL;
    }

    // Filter out special characters
    std::string out_str = stdout_stream.str();
    std::string* output = new std::string();

    if (out_str.empty())
      out_str = stderr_stream.str();
    
    for (const auto& c : out_str) {
      if (c < 32 && c != 10) continue;
      else (*output) += c;
    }

    // std::cout << "\nCOMMAND " << command << " : ";
    // for (const auto& c : output)
    //   std::cout << int(c) << ", ";

    return output;
  }
}

std::string* execute_with_output(const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& command : commands) {
    concatenated += command + ' ';
  }

  return execute_with_output(concatenated);
}

// Single line out
std::string* execute_with_output_single_line (const std::string& command) {
  /*
    A lot of functions expect a single
    line output, for which the extraction
    process looks the same.
  */

  // Get output
  std::string* command_out = execute_with_output(command);

  // If we receive a nullstr, return nullstr
  if (command_out == NULL)
    return NULL;

  // If we receive an empty string, return empty string
  if (command_out->length() == 0)
    return command_out;

  // Delimit string
  std::vector<std::string> lines = get_lines_from_string(*command_out);
  delete(command_out);

  // More than expected
  if (lines.size() > 1)
    return NULL;

  return new std::string(lines.front());
}

std::string* execute_with_output_single_line (const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& command : commands) {
    concatenated += command + ' ';
  }

  return execute_with_output_single_line(concatenated);
}

// Multi line out (Do not use)
std::vector<std::string> execute_with_output_multi_line(const std::string& command) {
  std::string* command_out = execute_with_output(command);

  // If we receive a nullstr, return an empty vector
  if (command_out == NULL)
    return {};

  std::vector<std::string> output = get_lines_from_string(*command_out);
  delete(command_out);
  return output;
}

// Do not use
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

// Get string from lines
std::string get_string_from_lines (const std::vector<std::string>& lines) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& line : lines) {
    concatenated += line + '\n';
  }

  return concatenated;
}

std::string* get_executable_path (const std::string& exec_name) {
  /*
    For applications, certain
    dependencies may be required
    for the application to run
    properly.

    To ensure these dependencies
    exist on the host machine,
    we can run the command,
    which executable_name
  */

  std::vector<std::string> commands({"which",exec_name});
  std::string* command_out = execute_with_output_single_line(commands);
  if (command_out == NULL) {
    std::string err_msg = "get_executable_path() ==> Executable does not exist: " + exec_name + '\n';
    perror(err_msg.c_str());
    return NULL;
  } return command_out;
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

// Check if a directory exists
bool dir_exists (const std::string& path) {
  std::vector<std::string> commands(
    {"[ -d", '\"' + path + '\"', "]"}
  );

  std::string* output = execute_with_output(commands);
  if (output == NULL) {
    std::string err_msg = "dir_exists() ==> Directory does not exist: " + path + '\n';
    perror(err_msg.c_str());
    return false;
  }
  delete(output);
  return true;
}

// Create directory in path
bool create_dir (const std::string& path, const std::string& name) {
  std::vector<std::string> commands(
    {"cd",path,"&&","mkdir",name}
  );

  int32_t err_code = execute_without_output(commands);
  if (err_code == 0)
    return true;
  
  std::string err_msg = "create_dir() ==> Could not create directory: " + name + ", in path: " + path + ", with Error Code: " + std::to_string(err_code) + '\n';
  perror(err_msg.c_str());
  return false;
}

// Check if a file exists
bool file_exists (const std::string& path) {
  std::ifstream file(path);

  if (file.good())
    return true;
  
  std::string err_msg = "file_exists() ==> File does not exist: " + path + '\n';
  // perror(err_msg.c_str());
  return false;
}

// Check if line exists in file
bool line_in_file_exists (const std::string& path, const std::string& s) {
  std::ifstream file(path);
  std::string line;

  if (file.is_open()) {
    while (std::getline(file, line)) {
      if (line == s)
        return true;
    } file.close();
    std::string err_msg = "line_in_file_exists() ==> Could not find line \"" + s + "\" in file: " + path + '\n';
    perror(err_msg.c_str());
  } else {
    std::string err_msg = "line_in_file_exists() ==> File does not exist: " + path + '\n';
    perror(err_msg.c_str());
  }

  return false;
}

// Append line to a file
bool append_line_to_file (const std::string& path, const std::string& s) {
  std::ifstream infile(path);
  std::string line;
  std::string last_line;

  if (infile.is_open()) {
    while (std::getline(infile, line))
      last_line = line;
    infile.close();
  } else {
    std::string err_msg = "append_line_to_file() ==> Failed to open file: " + path + '\n';
    perror(err_msg.c_str());
    return false;
  }

  std::ofstream file;
  file.open(path, std::ios::app);

  if (file.is_open()) {
    if (!last_line.empty())
      file << std::endl;
    file << s;
    file.close();
    return true;
  }

  std::string err_msg = "append_line_to_file() ==> Failed to open file: " + path + '\n';
  perror(err_msg.c_str());
  return false;
}

// Return position of line in file (return -1 otherwise)
int32_t line_pos_in_file (const std::string& path, const std::string& s) {
  std::ifstream file(path);
  std::string line;
  uint32_t pos = 0;

  if (file.is_open()) {
    while (std::getline(file, line)) {
      if (line == s) return pos;
      pos++;
    } file.close();
  } else {
    std::string err_msg = "line_pos_in_file() ==> Failed to open file: " + path + '\n';
    perror(err_msg.c_str());
    return -1;
  }

  std::string err_msg = "line_pos_in_file() ==> Could not find line \"" + s + "\" in file: " + path + '\n';
  perror(err_msg.c_str());
  return -1;
}

// Lock file using flock
bool lock_file (const std::string& path) {
  int file_descriptor = open(path.c_str(), O_CREAT | O_RDWR, 0666);

  if (file_descriptor == -1) {
    std::string err_msg = "lock_file() ==> Failed to open file: " + path + '\n';
    perror(err_msg.c_str());
    return false;
  }

  if (flock(file_descriptor, LOCK_EX) == -1) {
    std::string err_msg = "lock_file() ==> Failed to lock file: " + path + '\n';
    perror(err_msg.c_str());
    close(file_descriptor);
    return false;
  }

  return true;
}

// Unlock file using flock
bool unlock_file (const std::string& path) {
  int file_descriptor = open(path.c_str(), O_CREAT | O_RDWR, 0666);

  if (file_descriptor == -1) {
    std::string err_msg = "unlock_file() ==> Failed to open file: " + path + '\n';
    perror(err_msg.c_str());
    return false;
  }

  if (flock(file_descriptor, LOCK_UN) == -1) {
    std::string err_msg = "unlock_file() ==> Failed to unlock file: " + path + '\n';
    perror(err_msg.c_str());
    close(file_descriptor);
    return false;
  }

  return true;
}

// Clear file
bool clear_file (const std::string& path) {
  std::ofstream output_filestream;
  output_filestream.open(path, std::ofstream::out | std::ofstream::trunc);

  if (!output_filestream) {
    std::string err_msg = "clear_file() ==> Failed to open file: " + path + '\n';
    perror(err_msg.c_str());
    return false;
  }
  
  output_filestream.close();
  return true;
}

// Delete a file
bool remove_file (const std::string& path) {
  int status = remove(path.c_str());

  if (status != 0) {
    perror("Error deleting file");
    return false;
  } return true;
}

// Check external dependencies
bool check_external_dependencies (const std::vector<std::string>& dependencies) {
  /*
    Return true if all external
    dependencies satisfied.
  */

  std::string err_msg;

  for (const auto& dependency : dependencies) {
    std::string* dependency_path = get_executable_path(dependency);
    if (dependency_path == NULL)
      err_msg += "missing dependency: " + dependency + '\n';
    else delete(dependency_path);
  }

  if (!err_msg.empty()) {
    perror(err_msg.c_str());
    return false;
  } return true;
}

bool response_generator (const std::string& message) {
  std::cout << message << std::endl;
  std::string input;
  while (true) {
    std::cout << "Do you accept this ('y' or 'n'):\n";
    std::getline(std::cin, input);
    if (input == "y" || input == "Y")
      return true;
    else if (input == "n" || input == "N")
      return false;
  } return false;
}