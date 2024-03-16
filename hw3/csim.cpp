#include "csim.h"
#include <cstdint>
#include <iostream>
#include <cstdint>
#include <vector>

//helper function to chaeck if the the input number is a power of 2
bool check_two_power(int n) {
    while(n != 1) {
        if(n % 2 != 0) {
            return false;
        }
        n = n / 2;
    }
    return true;
}

//the main load function
void load(Cache &cache, uint32_t address, std::string replaceApproach) {
    //parse
    std::pair<uint32_t, uint32_t> parResult;
    parResult = parse(cache, address);
    Set& currentSet = cache.sets[parResult.second];
    int hitStatus = checkHit(cache, parResult.second, parResult.first);
    if (hitStatus != -1) {
        //if hit, update time
        cache.sets[parResult.second].slots[hitStatus].access_ts = cache.totalCycle; 
        cache.totalCycle += 1;
        cache.loadHit++;
    } else {
        uint32_t setPosition = checkSlotAvailability(currentSet);
        if (setPosition != uint32_t(-1)) {
            //If there's still space availble in current set
            Slot input = Slot{parResult.first, true, cache.totalCycle,cache.totalCycle, false};
            currentSet.slots[setPosition] = input;
        } else {
            if (replaceApproach == "fifo") {
                fifo(cache, currentSet, parResult.first);
            } else {
                lru(cache, currentSet, parResult.first);
            }
        }
        //update the total cycle as we write to the main memory
        cache.totalCycle += cache.sizeSlot*25;
        cache.loadMiss ++;
    }
    cache.loadCount++;
}

void store(Cache &cache, uint32_t address, std::string loadMain, std::string storemain, std::string replaceApproach) {
    //parse the function
    std::pair<uint32_t, uint32_t> parResult;
    parResult = parse(cache, address);
    int hitStatus = checkHit(cache, parResult.second, parResult.first);
    if (hitStatus != -1) {
        if (loadMain == "write-through") {
            writeThrough(cache, cache.sets[parResult.second], hitStatus);
        } else {
            writeBack(cache, cache.sets[parResult.second], hitStatus);
        }
        cache.totalCycle++;
        cache.storeHit++;
    } else {
        if (storemain == "no-write-allocate") {
            noWriteAllocate(cache);
        } else {
            writeAllocate(cache, replaceApproach, cache.sets[parResult.second], parResult.first, address, loadMain);
        }
        cache.storeMiss++;
    }
    cache.storeCount++;
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

int checkHit(Cache &cache, uint32_t index, uint32_t tag) {
    std::vector<Slot> slots = cache.sets[index].slots;
    for(uint32_t i = 0; i < cache.numSlot; i++) {
        if(slots[i].valid) {
            if(slots[i].tag == tag) {
                return i;
            }
        }
    }
    return -1;
}

int checkSlotAvailability(Set &set) {
    for (uint32_t i = 0; i < set.maxSlots; i++) {
        if(!(set.slots[i].valid)) {
            return i;
        }
    }
    return -1;
}

uint32_t fifo(Cache &cache,  Set &set, uint32_t tag) {
    uint32_t index = 0;
    uint32_t oldest = set.slots[0].load_ts;
    //check which block is imported oldest (with smallest load_ts)
    for(uint32_t i =0 ; i < set.maxSlots; i ++) {
        if (set.slots[i].load_ts < oldest) {
            index = i;
            oldest = set.slots[i].load_ts;
        }
    }
    //discard the original node
    discard(cache, set.slots[index]);
    //update
    set.slots[index] = {tag, true, cache.totalCycle, cache.totalCycle, false};
    return index;
}

uint32_t lru(Cache &cache, Set &set, uint32_t tag) {
    uint32_t index = 0;
    uint32_t oldest = set.slots[0].access_ts;
    //find the least recently used slot
    for(uint32_t i =0 ; i < set.maxSlots; i ++) {
        if (set.slots[i].access_ts < oldest) {
            index = i;
            oldest = set.slots[i].access_ts;
        }
    }
    //discard the original node
    discard(cache, set.slots[index]);
    //update
    set.slots[index] = {tag, true, cache.totalCycle, cache.totalCycle, false};
    return index;
}

void noWriteAllocate(Cache &cache) {
    cache.totalCycle += 100;
}

void writeAllocate(Cache &cache, std::string replaceApproach, Set &set, uint32_t tag, uint32_t address, std::string writeApproach) {
    //load the memory into cache
    load(cache, address, replaceApproach);
    int index = -1;
    cache.loadCount--;
    cache.loadMiss--;

    //find the index of the slot
    for (uint32_t i = 0; i < cache.numSlot; i++) {
        if (set.slots[i].tag == tag) {
            index = i;
            break;
        }
    }
    
    if (writeApproach == "write-back") {
        writeBack(cache, set, index);
    } else {
        writeThrough(cache, set, index);
    }
}

void writeThrough(Cache &cache, Set &set, uint32_t index) {
    set.slots[index].access_ts = cache.totalCycle;
    cache.totalCycle += 100;
}

void writeBack(Cache &cache, Set &set, uint32_t index) {
    set.slots[index].dirty = true;
    set.slots[index].access_ts = cache.totalCycle;
    cache.totalCycle++;
}

void discard(Cache &cache, Slot &slot) {
    if (slot.dirty) {
        cache.totalCycle += cache.sizeSlot * 25;
        slot.dirty = false;
    }
}