#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  for (;;) {
    // Print the shell prompt
    std::cout << "$ ";

    // Read user input
    std::string input;
    std::getline(std::cin, input);

    // Handle "exit 0"
    if (input == "exit 0") {
      return 0;
    }

    // Handle the "echo ..."
    if (input.find("echo ") == 0) {
      const int ECHO_LEN = 5;
      std::cout << input.substr(ECHO_LEN) << std::endl;
    } 
    // Handle the "type ..."
    else if (input.find("type ") == 0) {
      const int TYPE_LEN = 5;
      std::string argument = input.substr(TYPE_LEN);

      if (argument == "echo") {
        std::cout << "echo is a shell builtin" << std::endl;
      }
      else if (argument == "exit") {
        std::cout << "exit is a shell builtin" << std::endl;
      }
      else if (argument == "type") {
        std::cout << "type is a shell builtin" << std::endl;
      }

       else {
        std::cout << argument << ": not found" << std::endl;
       }
    }
    // If the argument is not recognized
    else {
      std::cout << input << ": command not found" << std::endl;
    }
  }
}
