#include "csim.h"
#include <cstdint>
#include <iostream>
#include <cstdint>
#include <vector>

bool check_two_power(int n) {
    while(n != 1) {
        if(n % 2 != 0) {
            return false;
        }
        n = n / 2;
    }
    return true;
}

void load(Cache &cache, uint32_t address, std::string replaceApproach) {
    //parse
    return;
    std::pair<uint32_t, uint32_t> parResult;
    parResult = parse(cache, address);
    Set currentSet = cache.sets[parResult.first];
    bool hitStatus = checkHit(cache, parResult.second, parResult.first);
    if (hitStatus) {
        //if hit, update time
    } else {
        uint32_t setStatus = checkSlotAvailability(currentSet);
        if (setStatus != uint32_t(-1)) {
            //If there's still space availble in current set
            Slot input = Slot{parResult.first, true, cache.totalCycle,cache.totalCycle};
            currentSet.slots[setStatus] = input;
        } else {
            if (replaceApproach == "fifo") {
                fifo(cache, parResult.first);
            } else {
                lru(cache, parResult.first);
            }
        }
    }
}

uint32_t get_two_power(uint32_t n) {
    uint32_t count = 0;
    while(n != 1) {
        n = n / 2;
        count ++;
    }
    return count;
}

std::pair<uint32_t, uint32_t> parse(Cache &cache, uint32_t address) {
    uint32_t num_bit_offset = get_two_power(cache.sizeSlot);
    uint32_t num_bit_index = get_two_power(cache.numSet);
    uint32_t tag = address >> (num_bit_offset + num_bit_index);
    uint32_t index = ( address >> (num_bit_offset) ) % (1 << num_bit_index);
    return std::make_pair(tag, index);
}

bool checkHit(Cache &cache, uint32_t index, uint32_t tag) {
    std::vector<Slot> slots = cache.sets[index].slots;
    for(std::vector<Slot>::iterator it = slots.begin(); it != slots.end(); it++) {
        if(it->valid) {
            if(it->tag == tag) {
                it->access_ts = cache.totalCycle;
                return true;
            }
        }
    }
    return false;
}

uint32_t checkSlotAvailability(Set &set) {
    for (int i = 0; i < set.maxSlots; i++) {
        if(!(set.slots[i].valid)) {
            return i;
        }
    }
    return -1;
}

void fifo(Cache &cache,  Set &set, uint32_t tag) {
    uint32_t index = 0;
    uint32_t oldest = set.slots[0].load_ts;
    for(int i =0 ; i < set.maxSlots; i ++) {
        if (set.slots[i].load_ts < oldest) {
            index = i;
            oldest = set.slots[i].load_ts;
        }
    }
    set.slots[index].tag = tag;
    set.slots[index].access_ts = cache.totalCycle;
    set.slots[index].load_ts = cache.totalCycle;
}

void lru(Cache &cache, uint32_t tag) {
    
}