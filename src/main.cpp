#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

// Function to split a string by a delimiter
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream iss(str);

    // Extract tokens separated by the delimiter
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true) {
    // Print the shell prompt
    std::cout << "$ ";

    // Read user input
    std::string input;
    std::getline(std::cin, input);

    // Handle "exit 0"
    if (input == "exit 0") {
      return 0;
    }

    // Handle the "echo ..." command
    if (input.find("echo ") == 0) {
      const int ECHO_LEN = 5;
      std::cout << input.substr(ECHO_LEN) << std::endl;
    }

    // Handle the "type ..." command
    else if (input.find("type ") == 0) {
      const int TYPE_LEN = 5;
      bool found = false;
      std::string argument = input.substr(TYPE_LEN);

      if(argument == "type" || argument == "exit" || argument == "echo"){
        found = true;
        std::cout << argument << " is a shell builtin" << std::endl;
      }
      // Search for the command in PATH directires
      else {
        std::string pathEnv = std::getenv("PATH");
        std::vector<std::string> directories = split(pathEnv, ':');

        for (const std::string& dir : directories) {
          std::string fullPath = dir + "/" + argument;
          
          if (std::filesystem::exists(fullPath)) {
            std::cout << argument << " is " << fullPath << std::endl;
            found = true;
            break;
          }
        }
      }

      // If the command was not found
      if (!found) {
          std::cout << argument << ": not found" << std::endl;
      }
    }

    else {
      std::cout << input << ": command not found" << std::endl;
    }
  }

  return 1;
}
