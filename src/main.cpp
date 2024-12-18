#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <filesystem>  // For std::filesystem::exists
#include <cstdlib>     // For std::getenv
#include <unistd.h>    // For getcwd and chdir
#include <limits.h>    // For PATH_MAX
#include <fstream>     // For file I/O

// Helper function: Splits a string into tokens based on a delimiter and quotes
std::vector<std::string> split(const std::string& str, char delimiter = ' ') {
  std::vector<std::string> tokens;
  bool insideQuotes = false;
  std::string token;

  for (char ch : str) {
    // Toggle the insideQuotes flag
    if (ch == '\'') {
      insideQuotes = !insideQuotes;
      continue;
    }

    // Check if the delimiter is inside quotes
    if (ch == delimiter && !insideQuotes) {
      if (!token.empty()) {    // ignore spaces
        tokens.push_back(token);
        token.clear();
      }
    } else {
      token += ch;
    }
  }

  // Add the last token if it's not empty
  if (!token.empty()) {
    tokens.push_back(token);
  }

  return tokens;
}

// Extract tokens separated by the delimiter
std::string get_path(const std::string& command) {
  std::string result = "";
  std::string path_env = std::getenv("PATH");
  std::vector<std::string> directories = split(path_env, ':');

  // Iterate over each directory in PATH
  for (const std::string& dir : directories) {
      std::string abs_path = dir + "/" + command;
      // Check if the command exists in this directory
      if (std::filesystem::exists(abs_path)) {
          result = abs_path;
          break;
      }
  }
    return result;
}


int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // Print the shell prompt and read user input
    std::cout << "$ ";
    std::string input;
    std::getline(std::cin, input);

    // Handle "exit 0"
    if (input == "exit 0") {
      return 0;
    }

    // Split the input into command and arguments
    std::vector<std::string> tokens = split(input);
    std::string command = tokens[0];

    // Handle the "echo" command
    if (command == "echo") {
      std::string content = "";

      // Combine all tokens
      for (size_t i = 1; i < tokens.size(); ++i) {
        if (!content.empty()) {
          content += " ";
        }
        content += tokens[i];
      }

      std::cout << content << std::endl;
    }

    // Handle the "type" command
    else if (command ==  "type") {
      const int TYPE_LEN = 5;
      std::string argument = input.substr(TYPE_LEN);

      // Check if the argument is a shell builtin
      if (argument == "type" || argument == "exit" || argument == "echo" || argument == "pwd") {
        std::cout << argument << " is a shell builtin" << std::endl;
      } else {
        std::string path = get_path(argument);

        // Check if command is found in PATH
        if (!path.empty()) {
          std::cout << argument << " is " << path << std::endl;
        } else {
          std::cout << argument << ": not found" << std::endl;
        }
      }
    }

    // Handle the "pwd" command
    else if (command == "pwd") {
      char cwd[PATH_MAX];

      getcwd(cwd, sizeof(cwd));
      std::cout << cwd << std::endl;
    }

    // Handle the "cd" command
    else if (command == "cd") {
      std::string& directory = tokens[1];

      // Handle the '~' character (user's home directory)
      if (directory == "~") {
        const char* home_dir = std::getenv("HOME");
        directory = home_dir;
      }
      // Attempt to change the directory
      if (chdir(directory.c_str()) != 0) {
        std::cerr << "cd: " << directory << ": No such file or directory" << std::endl;
      }
    }

    // Handle the "cat" command
    else if (command == "cat") {
      // Iterate over the arguments after "cat"
      for (size_t i = 1; i < tokens.size(); ++i) {
        std::string filename = tokens[i];
        std::ifstream inFile(filename);    // Open the file
        std::string line;

        // Print file content line by line
        while (std::getline(inFile, line)) {
          std::cout << line;
        }
        inFile.close();    // Close the file
      }

      std::cout << std::endl;
    }

    // Handle unrecognized commands
    else {
      std::string path = get_path(command);
      
      if (!path.empty()) {
        system(input.c_str());
      } else {
        std::cout << command << ": command not found" << std::endl;
      }
    }
  }

  return 1;
}
