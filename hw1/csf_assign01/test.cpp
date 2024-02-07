#include <iostream>
#include <cstdint>
#include <vector>
#include "bigint.h"
int main(){
    uint64_t left = 0x9f33ca9e0290d102;
    uint64_t right = 0x3efacbd8f95c7bb;
    BigInt first(left, false);
    BigInt second(right, true);
    BigInt temp = BigInt::subtract_magnitudes(first, second);
    std::cout << temp.big_string[0];
}