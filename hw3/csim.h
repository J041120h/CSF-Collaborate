#ifndef CSIM_H
#define CSIM_H

#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

struct Slot {
    uint32_t tag;
    bool valid;
    uint32_t load_ts,
             access_ts;
};

struct Set {
    std::vector<Slot> slots;
    uint32_t maxSlots;
};

struct Cache {
    std::vector<Set> sets;
    uint32_t loadCount;
    uint32_t storeCount;
    uint32_t loadHit;
    uint32_t loadMiss;
    uint32_t storeHit;
    uint32_t storeMiss;
    uint32_t totalCycle;
    uint32_t numSet;
    uint32_t numSlot;
    uint32_t sizeSlot;
};

//check if the n is the power of 2
bool check_two_power(int n);

//if the command is load, the function load will be called
void load(Cache &cache, uint32_t address, std::string replaceApproach);
//if the command is store, the function store will be called
void store(Cache &cache, uint32_t address, std::string loadMain, std::string storemain);

//helper functions for load and store

//parse the address based on the number of sets and size of slot
//return 1. tag 2. index (discard the offset)
std::pair<uint32_t, uint32_t> parse(Cache &cache, uint32_t address);

//check if the store or load is hit or miss
bool checkHit(Cache &cache, uint32_t index, uint32_t tag);

//check if there's additional slot
//return -1 if the slot is full, otherwise the index of the corresponding slot
uint32_t checkSlotAvailability(Set &set);

//if it's first in and first out
void fifo(Set &set, uint32_t tag);

//if it's least recently used
void lru(Set &set, uint32_t tag);

#endif