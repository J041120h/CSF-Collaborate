#include <atomic>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include "csim.h"
using std::cin;
using std::stringstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
int main(int argc, char* argv[]) {
    string line;
    string currentCommand;
    uint32_t memoryAddress;

    if(argc != 7) {
        cerr << "Invalid input number, some necessary part is missing!" << endl;
        exit(-1);
    }
    uint32_t num_set = std::stoi(argv[1]);
    if(!check_two_power(num_set)) {
        cerr << "Set number should be power of 2" << endl;
        exit(-1);
    }
    uint32_t num_block = std::stoi(argv[2]);
    if(!check_two_power(num_block)) {
        cerr << "Block nummber should be power of 2" << endl;
        exit(-1);
    }
    uint32_t num_bytes = std::stoi(argv[3]);
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

    vector<Set> CacheSets;
    for (uint32_t i = 0; i < num_set; i++) {
        vector<Slot> setSlots;
        for(uint32_t j = 0; j < num_block; j++) {
            Slot slot = {0, 0, 0, 0, false};
            setSlots.push_back(slot);
        }
        Set set = {setSlots, num_block};
        CacheSets.push_back(set);
    }
    Cache cache = {CacheSets, 0, 0, 0, 0, 0, 0, 0, num_set, num_block, num_bytes};


    while (std::getline(cin, line)) {
        if (line.empty()) {
            break; // Stop if the line is empty
        } else {
            stringstream input;
            input << line;
            if (input >> currentCommand >> std::hex >> memoryAddress) {
                // Successfully extracted command and memory address
                if (currentCommand == "l" ) {
                    load(cache, memoryAddress, lru_or_fifo);
                } else {
                    store(cache, memoryAddress, store_memory_write, store_cache_miss, lru_or_fifo);
                }
                
            } else {
                // Handle error: Could not extract both values
                std::cout << "Error: Invalid input format." << std::endl;
                break;
            }
        }
    }

    //print the out put
    std::cout << "Total loads: " << cache.loadCount << std::endl;
    std::cout << "Total stores: " << cache.storeCount << std::endl;
    std::cout << "Load hits: " << cache.loadHit << std::endl;
    std::cout << "Load misses: " << cache.loadMiss << std::endl;
    std::cout << "Store hits: " << cache.storeHit << std::endl;
    std::cout << "Store misses: " << cache.storeMiss << std::endl;
    std::cout << "Total cycles: " << cache.totalCycle << std::endl;

    double loadHitRate = (double)cache.loadHit/cache.loadCount;
    double storeHitRate = (double)cache.storeHit/cache.storeCount;
    std::cout << std::endl;
    std::cout<< "Hit Rate: " << loadHitRate << std::endl;
    std::cout<< "Store Rate: " << storeHitRate << std::endl;
}



