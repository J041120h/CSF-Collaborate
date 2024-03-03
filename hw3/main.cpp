#include <atomic>
#include <cstdint>
#include <iostream>
#include <sstream>
#include "csim.h"
using std::cin;
using std::stringstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
int main(int argc, char* argv[]) {
    if(argc != 7) {
        cerr << "Invalid input number, some necessary part is missing!" << endl;
        exit(-1);
    }
    int num_set = std::stoi(argv[1]);
    if(!check_two_power(num_set)) {
        cerr << "Set number should be power of 2" << endl;
        exit(-1);
    }
    int num_block = std::stoi(argv[2]);
    if(!check_two_power(num_block)) {
        cerr << "Block nummber should be power of 2" << endl;
        exit(-1);
    }
    int num_bytes = std::stoi(argv[3]);
    if(!check_two_power(num_bytes)) {
        cerr << "Byte nummber should be power of 2" << endl;
        exit(-1);
    }
    string store_cache_miss = argv[4];
    if(store_cache_miss != "write-allocate" && store_cache_miss != "no-write-allocate") {
        cerr << "Undefined command" << endl;
        exit(-1);
    }
    string store_memory_write = argv[5];
    if(store_memory_write != "write-through" && store_memory_write != "write-back") {
        cerr << "Undefined command" << endl;
        exit(-1);
    }
    if(store_cache_miss == "no-write-allocate" && store_memory_write == "write-back") {
        cerr << "Can't have no-write-allocate and write-back together" << endl;
        exit(-1);
    }
    string lru_or_fifo = argv[6];
    if(lru_or_fifo != "lru" && lru_or_fifo != "fifo") {
        cerr << "Undefined command" << endl;
        exit(-1);
    }


}




void read_input() {
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