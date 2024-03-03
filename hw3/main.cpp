#include <iostream>
#include <string>
#include "csim.h"
#include "reader.h"
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
int main(int argc, char* argv[]) {
    if(argc == 7) {
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
    if(store_memory_write != "write through" && store_memory_write != "write back") {
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

bool check_two_power(int n) {
    while(n != 0) {
        if(n % 2 != 0) {
            return false;
        }
        n = n / 2;
    }
    return true;
}
