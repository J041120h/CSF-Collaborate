#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include "bigint.h"

BigInt::BigInt()
  // TODO: initialize member variables
{
  bit_string.push_back(0);
  negative = false;
}

BigInt::BigInt(uint64_t val, bool negative)
  // TODO: initialize member variables
{
  bit_string.push_back(val);
  this->negative = negative;
}

BigInt::BigInt(std::initializer_list<uint64_t> vals, bool negative)
  // TODO: initialize member variables
{
  this->negative = negative;
  for(auto v = vals.begin(); v != vals.end(); v++){
    bit_string.push_back(*v);  //bit_string[0] is the most insignificant
  }
}

BigInt::BigInt(const BigInt &other)
  // TODO: initialize member variables
{
  this->bit_string = other.bit_string;
  this->negative = other.negative;
}

BigInt::~BigInt()
{
  std::vector<uint64_t> temp;
  temp.push_back(0);
  this->bit_string = temp;
  this->negative = false;
}

BigInt &BigInt::operator=(const BigInt &rhs)
{
    this->bit_string = rhs.bit_string;
    this->negative = rhs.negative;
    return *this;
}

bool BigInt::is_negative() const
{
  // TODO: implement
  return negative;
}

const std::vector<uint64_t> &BigInt::get_bit_vector() const {
  // TODO: implement
  return bit_string;
}

uint64_t BigInt::get_bits(unsigned index) const
{
  // TODO: implement
  uint64_t size = bit_string.size();
  if(index >= size){
    return 0;
  }
  return bit_string[index];
}

BigInt BigInt::operator+(const BigInt &rhs) const
{
  // TODO: implement
  BigInt ans;
  if(this->negative && rhs.negative){
    ans = BigInt::add_magnitudes(*this, rhs);
    ans.negative = true;
  }
  else if(!this->negative && !rhs.negative){
    ans = BigInt::add_magnitudes(*this, rhs);
    ans.negative = false;
  }
  else{
    ans = BigInt::subtract_magnitudes(*this, rhs);
    if(BigInt::compare_magnitudes(*this, rhs) > 0){
      ans.negative = this->negative;
    }
    else if(BigInt::compare_magnitudes(*this, rhs) < 0){
      ans.negative = rhs.negative;
    }
  }
  return ans;
}

BigInt BigInt::operator-(const BigInt &rhs) const
{
  // TODO: implement
  // Hint: a - b could be computed as a + -b
  BigInt temp = -rhs;
  return *this + temp;
}

BigInt BigInt::operator-() const
{
  // TODO: implement
  BigInt ans = *this;
  ans.negative = !(this->negative);
  return ans;
}

bool BigInt::is_bit_set(unsigned n) const
{
  // TODO: implement
  int position = n / 64;
  int bit_position = n % 64;
  uint64_t num = this->get_bits(position);
  if((num >> bit_position) % 2 == 1){
    return true;
  }
  return false;
}

BigInt BigInt::operator<<(unsigned n) const
{
  // TODO: implement
  if(negative){
    throw std::invalid_argument("Trying to left shift a negative value");
  }
  std::vector<uint64_t> left_shift;
  int num_64 = n / 64;
  int bit_num = n % 64;
  for(int i = 0; i <= num_64; i++){
    left_shift.push_back(0);  //add zeros to the end of the bit string
  }
  for(int i = 0; i < (int)(bit_string.size()); i++){
    left_shift[num_64 + i] += this->get_bits(i) << bit_num;
    if(bit_num == 0){
      left_shift.push_back(0);
    }
    else{
      left_shift.push_back((this->get_bits(i) >> (64 - bit_num)));
    }
  }
  BigInt ans;
  ans.bit_string = left_shift;
  return ans;
}

BigInt BigInt::operator*(const BigInt &rhs) const
{
  // TODO: implement
  std::vector<int> power_list = BigInt::get_2power_sum(rhs);
  BigInt ans;
  BigInt lhs = *this;
  if(this->negative){
    lhs = -(*this);
  }
  for(auto v = power_list.begin(); v != power_list.end(); v++){
    ans = ans + (lhs << (*v));
  }
  if(this->negative != rhs.negative){
    ans.negative = true;
  }
  return ans;
}

BigInt BigInt::operator/(const BigInt &rhs) const 
{
  // TODO: implement
  BigInt zero;
  if(rhs == zero){
    throw std::invalid_argument("Attempt to divide by 0");
  }
  BigInt labsolute;
  labsolute = *this;
  if(this->negative){
    labsolute = -(*this);
  }
  BigInt rabsolute;
  rabsolute = rhs;
  if(rhs.negative){
    rabsolute = -rhs;
  }
  BigInt ans;
  ans = BigInt::divide_answer(labsolute, rabsolute, zero, labsolute);
  if(this->negative != rhs.negative){
    ans.negative = true;
  }
  return ans;
}

int BigInt::compare(const BigInt &rhs) const
{
  // TODO: implement
  if(this->negative && rhs.negative){  //all negative
    return -(compare_magnitudes(*this, rhs));
  }
  else if(!this->negative && !rhs.negative){
    return compare_magnitudes(*this, rhs);
  }
  else{
    if(this->negative){
      return -1;
    }
    else{
      return 1;
    }
  }
}

std::string BigInt::to_hex() const
{
  // TODO: implement
  std::stringstream ss;
  if(this->negative){
    ss << "-";
  }
  for(auto v = bit_string.rbegin(); v != bit_string.rend(); v++){
    if(v == bit_string.rbegin()){
      ss << std::hex << *v;
    }
    else{
      ss << std::hex << std::setfill('0') << std::setw(16) << *v;
    }
  }
  return ss.str();
}

std::string BigInt::to_dec() const
{
  // TODO: implement
  std::vector<uint64_t> list;
  BigInt zero;
  if(*this == zero){
    std::string s = "0";
    return s;
  }
  BigInt ten(10, false);
  BigInt temp = *this;
  if(this->negative){
    temp = -(*this);
  }
  while(temp != zero){
    BigInt quotient = temp / ten;
    BigInt ans = temp - (quotient * ten);
    list.push_back(ans.bit_string[0]);
    temp = temp / ten;
  }
  std::stringstream ss;
  if(this->negative){
    ss << "-";
  }
  for(auto v = list.rbegin(); v != list.rend(); v++){
    ss << *v;
  }
  return ss.str();
}

BigInt BigInt::add_magnitudes(const BigInt& lhs, const BigInt& rhs){
  std::vector<uint64_t> plus_result;
  for(int index = 0; index < (int)std::max(lhs.bit_string.size(), rhs.bit_string.size()); index++){
    if((int)plus_result.size() < index + 1){
      plus_result.push_back(0);
    }
    uint64_t sum = plus_result[index] + lhs.get_bits(index) + rhs.get_bits(index);
    if(sum < plus_result[index] + lhs.get_bits(index)){ //overflow condition
      plus_result.push_back(1);
    }
    plus_result[index] = sum;
  }
  BigInt ans;
  ans.bit_string = plus_result;
  ans.negative = false;
  return ans;
}

BigInt BigInt::subtract_magnitudes(const BigInt& lhs, const BigInt& rhs){
  std::vector<uint64_t> subtract_result;
  for(int index = 0; index < (int)std::max(lhs.bit_string.size(), rhs.bit_string.size()); index++){
    uint64_t larger = lhs.get_bits(index);
    uint64_t smaller = rhs.get_bits(index);
    if(BigInt::compare_magnitudes(lhs, rhs) == 0){
      return BigInt();
    }
    else if (BigInt::compare_magnitudes(lhs, rhs) < 0){
      larger = rhs.get_bits(index);
      smaller = lhs.get_bits(index);
    }
    if((int)subtract_result.size() < index + 1){
      subtract_result.push_back(0);
    }
    uint64_t diff = subtract_result[index] + larger - smaller;
    if(larger == 0 && smaller != 0){
      subtract_result.push_back(-1);
    }
    else if(diff > subtract_result[index] + larger){
      subtract_result.push_back(-1);
    }
    subtract_result[index] = diff;
  }
  BigInt ans;
  ans.bit_string = subtract_result;
  ans.negative = false;
  return ans;
}

int BigInt::compare_magnitudes(const BigInt& lhs, const BigInt& rhs){
  int lsize = lhs.bit_string.size();
  int rsize = rhs.bit_string.size();
  for(auto v = lhs.bit_string.rbegin(); v != lhs.bit_string.rend(); v++){
    if(*v != 0){
      break;
    }
    lsize--;
  }
  for(auto v = rhs.bit_string.rbegin(); v != rhs.bit_string.rend(); v++){
    if(*v != 0){
      break;
    }
    rsize--;
  }
  if(lsize != rsize){
    if(lsize > rsize){
      return 1;
    }
    return -1;
  }
  for(int i = lsize - 1; i >= 0; i--){
    if(lhs.bit_string[i] != rhs.bit_string[i]){
      if(lhs.bit_string[i] - rhs.bit_string[i] > lhs.bit_string[i]){
        return -1;
      }
      return 1;
    }
  }
  return 0;
}

std::vector<int> BigInt::get_2power_sum(const BigInt& lhs){
  std::vector<int> ans;
  for(int i = 0; i < (int)lhs.bit_string.size(); i++){
    for(int j = 0; j < 64; j++){
      if((lhs.get_bits(i) >> j) % 2 == 1){
        ans.push_back(i * 64 + j);
      }
    }
  }
  return ans;
}

BigInt BigInt::div_by_2() const{
  std::vector<uint64_t> div_result;
  for(int i = 0; i < (int)bit_string.size(); i++){
    div_result.push_back(this->get_bits(i) >> 1);
    if(i < (int)bit_string.size() - 1){
      div_result[i] += ((this->get_bits(i + 1) % 2) << 63);
    }
  }
  BigInt ans;
  ans.bit_string = div_result;
  return ans;
}

BigInt BigInt::divide_answer(const BigInt& lhs, const BigInt& rhs, BigInt lower, BigInt upper){
  BigInt one(1, false);
  if(upper == lower || upper - lower == one){
    return lower;
  }
  BigInt temp = lower + upper;
  BigInt trial = temp.div_by_2();
  BigInt actual = trial * rhs;
  if(actual == lhs){
    return trial;
  }
  else if(actual < lhs){
    return BigInt::divide_answer(lhs, rhs, trial, upper);
  }
  else{
    return BigInt::divide_answer(lhs, rhs, lower, trial);
  }
}

