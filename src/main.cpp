#include <iostream>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "$ ";
  std::string input;
  while (true)
  {
    
  std::getline(std::cin, input);
  std::cout << input << ": not found" << std::endl;
  std::cout << "$ ";

  }
  
}