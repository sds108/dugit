/*
  In this file, one can find
  all of the necessary standard
  library includes as well as
  custom declarations that are
  commonly used everywhere in
  this project.
*/

#ifndef INCLUDE_H
#define INCLUDE_H

// All of the includes necessary
#include <string>
#include <cstring>
#include <iostream>
#include <exception>
#include <limits>
#include <vector>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <map>
#include <functional>
#include <chrono>
#include <iomanip>

// Useful functions
char* get_shell();
std::string* get_ppid();
std::string* get_cwd();
std::string* get_pwd();
std::string* get_executable_path(const std::string& exec_name);

// Run command
int32_t execute_without_output(const std::string& command);
int32_t execute_without_output(const std::vector<std::string>& commands);

// Run command and get output
std::string* execute_with_output(const std::string& command);
std::string* execute_with_output(const std::vector<std::string>& commands);

// Single line out
std::string* execute_with_output_single_line(const std::string& command);
std::string* execute_with_output_single_line(const std::vector<std::string>& commands);

// Multi line out (Do not use)
std::vector<std::string> execute_with_output_multi_line(const std::string& command);
std::vector<std::string> execute_with_output_multi_line(const std::vector<std::string>& commands);

// Get lines from string
std::vector<std::string> get_lines_from_string(const std::string& s);

// Get string from lines
std::string get_string_from_lines(const std::vector<std::string>& lines);

// Trim front of a string
bool string_trim_front(std::string& s, const unsigned long long len);
bool strings_trim_fronts(std::vector<std::string>& lines, const unsigned long long len);

// Trim rear of a string
bool string_trim_rear(std::string& s, const unsigned long long len);
bool strings_trim_rears(std::vector<std::string>& lines, const unsigned long long len);

// Check if a directory exists
bool dir_exists(const std::string& path);

// Create directory in path
bool create_dir(const std::string& path, const std::string& name);

// Check if a file exists
bool file_exists(const std::string& path);

// Check if line exists in file
bool line_in_file_exists(const std::string& path, const std::string& s);

// Append line to a file
bool append_line_to_file(const std::string& path, const std::string& s);

// Return position of line in file (return -1 otherwise)
int32_t line_pos_in_file(const std::string& path, const std::string& s);

// Lock file using flock
bool lock_file(const std::string& path);

// Unlock file using flock
bool unlock_file(const std::string& path);

// Clear file
bool clear_file(const std::string& path);

// Delete a file
bool remove_file(const std::string& path);

// Check external dependencies
bool check_external_dependencies(const std::vector<std::string>& dependencies);

#endif