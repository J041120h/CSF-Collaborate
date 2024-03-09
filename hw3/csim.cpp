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
        for (std::vector<Slot>::iterator i = currentSet.slots.begin(); i != currentSet.slots.end(); i++ ) {
            if (i->valid) {
                if (i->tag == parResult.first) {
                    //the slot is already available in the set
                    i->access_ts = cache.totalCycle;
                } 
            }
        }
    } else {
        uint32_t setStatus = checkSlotAvailability(currentSet);
        if (setStatus != uint32_t(-1)) {
            //If there's still space availble in current set
            Slot input = Slot{parResult.first, true, cache.totalCycle,cache.totalCycle};
            currentSet.slots[setStatus] = input;
        } else {
            if (replaceApproach == "fifo") {
                fifo(cache, currentSet, parResult.first);
            } else {
                lru(cache, currentSet, parResult.first);
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
void lru(Cache &cache, Set &set, uint32_t tag) {
    std::vector<Slot> slots = set.slots;
    uint32_t min_cycle = slots[0].access_ts;
    uint32_t min_index = 0;
    for(int i = 0; i < set.maxSlots; i++) {
        if(slots[i].access_ts < min_cycle) {
            min_cycle = slots[i].access_ts;
            min_index = i;
        }
    }
    //discard slots[min_index], update cycle
    slots[min_index] = {tag, true, cache.totalCycle, cache.totalCycle};
}