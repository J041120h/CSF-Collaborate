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
        uint32_t setStatus = checkSlotAvailability(currentSet);
        if (setStatus != uint32_t(-1)) {
            //If there's still space availble in current set
            Slot input = Slot{parResult.first, true, cache.totalCycle,cache.totalCycle, false};
            currentSet.slots[setStatus] = input;
        } else {
            if (replaceApproach == "fifo") {
                fifo(cache, currentSet, parResult.first);
            } else {
                lru(cache, currentSet, parResult.first);
            }
        }
        cache.totalCycle += cache.sizeSlot*25;
        cache.loadMiss ++;
    }
    cache.loadCount++;
}

void store(Cache &cache, uint32_t address, std::string loadMain, std::string storemain, std::string replaceApproach) {
    std::pair<uint32_t, uint32_t> parResult;
    parResult = parse(cache, address);
    Set& currentSet = cache.sets[parResult.second];
    int hitStatus = checkHit(cache, parResult.second, parResult.first);
    if (hitStatus != -1) {
        if (loadMain == "write-through") {
            cache.totalCycle++;
            writeThrough(cache, cache.sets[parResult.second], hitStatus);
        } else {
            cache.totalCycle++;
            writeBack(cache, cache.sets[parResult.second], hitStatus);
        }
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
    for(uint32_t i =0 ; i < set.maxSlots; i ++) {
        if (set.slots[i].load_ts < oldest) {
            index = i;
            oldest = set.slots[i].load_ts;
        }
    }
    //discard the original node
    discard(cache, set.slots[index]);
    //update
    set.slots[index].tag = tag;
    set.slots[index].access_ts = cache.totalCycle;
    set.slots[index].load_ts = cache.totalCycle;
    return index;
}

uint32_t lru(Cache &cache, Set &set, uint32_t tag) {
    uint32_t index = 0;
    uint32_t oldest = set.slots[0].access_ts;
    for(uint32_t i =0 ; i < set.maxSlots; i ++) {
        if (set.slots[i].access_ts < oldest) {
            index = i;
            oldest = set.slots[i].access_ts;
        }
    }
    //discard the original node
    bool dirty = false;
    if (set.slots[index].dirty) {
        dirty = true;
    }
    set.slots[index] = {tag, true, cache.totalCycle, cache.totalCycle, false};
    if (dirty) {
        cache.totalCycle += cache.sizeSlot * 25;
    }
    return index;
}

void noWriteAllocate(Cache &cache) {
    cache.totalCycle += 25;
}

void writeAllocate(Cache &cache, std::string replaceApproach, Set &set, uint32_t tag, uint32_t address, std::string writeApproach) {
    
    load(cache, address, replaceApproach);
    cache.loadCount--;
    cache.loadMiss--;
    if (writeApproach == "write-back") {
        int index = -1;
        for (int i = 0; i < cache.numSlot; i++) {
            if (set.slots[i].tag == tag) {
                index = i;
                break;
            }
        }
        set.slots[index].dirty = true;
        cache.totalCycle++;
    } else {
        cache.totalCycle += cache.sizeSlot * 25;
    }
    //uint32_t index = -1;
    //uint32_t setStatus = checkSlotAvailability(set);
    //if (setStatus != uint32_t(-1)) {
        //If there's still space availble in current set
        //Slot input = Slot{tag, true, cache.totalCycle,cache.totalCycle, true};
        //set.slots[setStatus] = input;
    //} else {
        //if (replaceApproach == "fifo") {
            //index = fifo(cache, set, tag);
        //} else {
            //index =lru(cache, set, tag);
        //}
        //set.slots[index].dirty = true;
    //}
    //cache.totalCycle += cache.sizeSlot/4*100 + 1;
}

void writeThrough(Cache &cache, Set &set, uint32_t index) {
    set.slots[index].access_ts = cache.totalCycle;
    cache.totalCycle += cache.sizeSlot * 25;
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