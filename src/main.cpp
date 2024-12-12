#include <iostream>
#include <string>

int main() {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    for (;;) {
        std::cout << "$ ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit 0") {
            return 0;
        }

        if (input.rfind("echo ", 0) == 0) {
            std::cout << input.substr(5) << std::endl;
        } else {
            std::cout << input << ": command not found" << std::endl;
        }
    }
}
