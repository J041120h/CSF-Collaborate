#include <iostream>
#include <cstdint>
#include <vector>
int main(){
    uint64_t left = 0x9f33ca9e0290d102;
    uint64_t right = 0x3efacbd8f95c7bb;
    std::cout << std::hex << left - right << std::endl;
    std::vector<uint64_t> temp;
    temp.push_back(-1);
    std::cout << temp[0];
}