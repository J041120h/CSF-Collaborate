#include <atomic>
#include <cstdint>
#include <iostream>
#include<sstream>

using std::cin;
using std::stringstream;
using std::string;

int main() {
    return 0;
}


void reader() {
    string line;
    string currentCommand;
    uint32_t memoryAddress;
    while (std::getline(cin, line)) {
        if (line.empty()) {
            break; // Stop if the line is empty
        } else {
            stringstream input;
            input << line;
            if (input >> currentCommand >> memoryAddress) {
                // Successfully extracted command and memory address
                std::cout << "Command: " << currentCommand << ", Memory Address: " << memoryAddress << std::endl;
            } else {
                // Handle error: Could not extract both values
                std::cout << "Error: Invalid input format." << std::endl;
            }
        }

    }
}