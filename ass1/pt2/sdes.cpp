#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bitset>
#include <stdint.h>   // for uint32_t
#include <limits.h>   // for CHAR_BIT
#include <assert.h>

using namespace std;

int IP(int input) {
  int output = 0;

  vector<int> IP_table = {2,6,3,1,4,8,5,7};

  for (auto &p:IP_table) {
    output = (output<<1) | ((input >> ( 8-p ) ) & 1);
  }

  return output;
}

int IPi(int input) {
  int output = 0;
  vector<int> IPi_table = {4,1,3,5,7,2,8,6};
  for (auto &p:IPi_table) {
    output = (output<<1) | ((input >> ( 8-p ) ) & 1);
  }
  return output;
}

int P10(int input) {
  int output;
  vector<int> P10_table = {3,5,2,7,4,10,1,9,8,6};

  for (auto &p:P10_table) {
    output = (output<<1) | ((input >> ( 10-p ) ) & 1);
  }

  return output;
}

int P8(int input) {
  int output;
  vector<int> P8_table = {6,3,7,4,8,5,10,9};
  for (auto &p:P8_table) {
    output = (output<<1) | ((input >> (10-p)) & 1);
  }

  return output;
}

int P4(int input) {
  int output = 0;
  vector<int> P4_table = {2,4,3,1};
  for (auto &p:P4_table) {
    output = (output<<1) | ((input >> (4-p)) & 1);
  }

  return output;
}

int leftShift(int input) {
  return ((input << 1) | ((input >> sizeof(input)) & 1));
}

int EP(int input) {
  int output = 0;
  vector<int> EP_table = {4,1,2,3,2,3,4,1};
  for (auto &p:EP_table) {
    output = (output<<1) | ((input >> (4-p)) & 1);
  }
  return output;
}

int sBox(int input, short int sbox) {
  int output = 0;
  int row;
  int col;

  row = ((input >> 3) & 1);
  row = (row<<1) | ((input >> 0) & 1);

  col = ((input >> 2) & 1);
  col = (col<<1) | ((input >> 1) & 1);

  vector<vector<int>> sbox0;
  sbox0.push_back({01,00,11,10});
  sbox0.push_back({11,10,01,00});
  sbox0.push_back({00,10,01,11});
  sbox0.push_back({11,01,11,10});

  vector<vector<int>> sbox1;
  sbox1.push_back({00,01,10,11});
  sbox1.push_back({10,00,01,11});
  sbox1.push_back({11,00,01,00});
  sbox1.push_back({10,01,00,11});

  if (sbox == 0) {
    output = sbox0[row][col];
  }
  if (sbox == 1) {
    output = sbox1[row][col];
  }
  return output;
}

std::vector<int> gen_keys(int key) {
  int p8input;
  int combine;
  vector<int> keys;
  int output = P10(key);

  // split output into tow parts left and right

  int left  = 0b1111100000 & output;
  left >>=5;
  int right = 0b0000011111 & output;

  // leftshift by 1
  left  = leftShift(left);
  right = leftShift(right);

  combine = left;
  combine = (combine<<5) | right;

  int key_1 = P8(combine);
  //key_1 &= 0b0011111111;
  keys.push_back(key_1);

  // leftshift again by 2
  right = leftShift(right);
  left  = leftShift(left);
  right = leftShift(right);
  left  = leftShift(left);


  left  &= 0b0000011111;
  right &= 0b0000011111;

  combine = left;
  combine = (combine<<5) | right;

  int key_2 = P8(combine);
  //key_2 &= 0b0011111111;
  keys.push_back(key_2);

  return keys;
}

int fBox(int input, int key) {
  int combine = 0;
  int output = 0;
  int left;
  int right;
  output = EP(input);

  // XOR key and 8 bits of EP
  output ^= key;

  left = 0b11110000 & output;
  left >>= 4;
  right = 0b00001111 & output;

  left = sBox(left,0);
  right = sBox(right,1);
  left &= 0b00000011;
  right &= 0b00000011;

  combine = left;
  combine = (combine<<2) | right;

  output = P4(combine);

  return output;
}

int encrypt(int input, vector<int> keys) {
  int output = 0;
  int tmp_left;
  output = IP(input);

  // split into 4bits
  int left = 0b11110000 & output;
  left >>= 4;
  int right = 0b00001111 & output;

  left ^= fBox(right,keys[0]);

  // swap
  tmp_left = left;
  left = right;
  right = tmp_left;

  left ^= fBox(right,keys[1]);

  output = left;
  output = (output<<4) | right;

  output = IPi(output);


  return output;
}

int decrypt(int input, vector<int> keys) {
  int output = 0;
  int tmp_left;
  output = IP(input);

  // split into 4bits
  int left = 0b11110000 & output;
  left >>= 4;
  int right = 0b00001111 & output;

  left ^= fBox(right,keys[1]);

  // swap
  tmp_left = left;
  left = right;
  right = tmp_left;

  left ^= fBox(right,keys[0]);

  output = left;
  output = (output<<4) | right;

  output = IPi(output);


  return output;
}

int tripleSDESenc(int msg, int key1,int key2) {
  vector<int> keys1 = gen_keys(key1);
  vector<int> keys2 = gen_keys(key2);

  int output = 0;
  output = encrypt(encrypt(encrypt(msg,keys1),keys2),keys1);
  return output;
}

int tripleSDESdec(int msg, int key1,int key2) {
  vector<int> keys1 = gen_keys(key1);
  vector<int> keys2 = gen_keys(key2);
  int output = 0;
  output = decrypt(decrypt(decrypt(msg,keys1),keys2),keys1);
  return output;
}

int score_msg(msg) {

  return 0;
}

int main() {

  

  return 0;
}
