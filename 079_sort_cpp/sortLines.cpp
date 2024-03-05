#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

int main(int argc, char* argv[]) {
  

  if (argc == 1) {
    std::vector<std::string> lines; 
    std::string line; 
    while (std::getline(std::cin, line)) {
      lines.push_back(line); 
    }
    std::sort(lines.begin(), lines.end()); 

    std::vector<std::string>::iterator l = lines.begin();
    while(l != lines.end()){
      std::cout << *l << std::endl;
      ++l;
    }
  } else {
    for (int i = 1; i < argc; i++) { 
      std::ifstream file(argv[i]); 

      if (!file) {
        std::cerr << "Cannot open file: " << argv[i] << std::endl; 
        return EXIT_FAILURE; 
      }

      std::vector<std::string> lines; 
      std::string line; 
      while (std::getline(file, line)) {
        lines.push_back(line); 
      }
      std::sort(lines.begin(), lines.end()); 

      std::vector<std::string>::iterator l = lines.begin();
      while(l != lines.end()){
        std::cout << *l << std::endl;
        ++l;
      }
      file.close();
    }
  }

  return EXIT_SUCCESS; 
}