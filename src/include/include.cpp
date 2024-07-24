#include "include.h"

std::string get_cwd () {
  char buffer[4096];
  if (getcwd(buffer, sizeof(buffer)) == NULL)
    return nullstr;
  return buffer;
}

// Run command
int execute_without_output (const std::string& command) {
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) {
    perror("popen");
    return 1;
  }

  int exitCode = pclose(pipe);
  if (exitCode == -1) {
    perror("pclose");
    return 1;
  }

  return 0;
}

int execute_without_output (const std::vector<std::string>& commands) {
  // Concatenate the command vector
  std::string concatenated;

  for (const auto& command : commands) {
    concatenated += command + ' ';
  }

  return execute_without_output(concatenated);
}

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

  if (lines.empty() || lines.size() > 1)
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

std::string get_executable_path (const std::string& exec_name) {
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

  return execute_with_output_single_line(commands);
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
    {"[ -d", '\"' + path + '\"', "]", "&&", "echo \"true\"", "||", "echo \"false\""}
  );

  std::string output = execute_with_output_single_line(commands);

  if (output == "true")
    return true;
  else return false;
}

// Create directory in path
bool create_dir (const std::string& path, const std::string& name) {
  std::vector<std::string> commands(
    {"cd",path,"&&","mkdir",name}
  );

  return execute_without_output(commands);
}

// Check if a file exists
bool file_exists (const std::string& path) {
  std::ifstream infile(path);

  if (infile.good())
    return true;
  else return false;
}

// Check if line exists in file
bool line_in_file_exists (const std::string& path, const std::string& s) {
  std::ifstream infile(path);
    std::string line;

  if (infile.is_open()) {
    while (std::getline(infile, line)) {
      if (line == s)
        return true;
    } infile.close();
  } else perror("ifstream open fail");

  return false;
}

// Append line to a file
bool append_line_to_file (const std::string& path, const std::string& s) {
  std::ofstream file;
  file.open(path, std::ios::app);

  if (file.is_open()) {
      file << std::endl << s;
      file.close();
      return true;
  }
  
  return false;
}

