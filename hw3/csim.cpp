#include "csim.h"
#include <cstdint>
#include <iostream>

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
                fifo(currentSet, parResult.first);
            } else {
                lru(currentSet, parResult.first);
            }
        }
    }


}