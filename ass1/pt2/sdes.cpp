#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <bitset>
#include <assert.h>
#include <ctime>
#include <time.h>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

using namespace std;

std::mutex mtx;
std::condition_variable cv;
bool ready = false;


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
  int combine;
  vector<int> keys;
  int output = P10(key);
  // split output into tow parts left and right
  int left  = 0b1111100000 & output;left >>=5;
  int right = 0b0000011111 & output;

  // leftshift by 1
  left  = leftShift(left)&0b0000011111;
  right = leftShift(right)&0b0000011111;

  combine = (left<<5)&0b1111100000;
  combine |= right;

  int key_1 = P8(combine)&0b0011111111;
  keys.push_back(key_1);

  // leftshift again by 2
  right = leftShift(right);
  left  = leftShift(left);
  right = leftShift(right)&0b0000011111;
  left  = leftShift(left)&0b0000011111;

  combine = (left<<5)&0b1111100000;
  combine |= right;

  int key_2 = P8(combine)&0b0011111111;
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

  left = (0b11110000 & output)>>4;
  right = 0b00001111 & output;

  left = sBox(left,0)&0b00000011;
  right = sBox(right,1)&0b00000011;

  combine = (left<<2);
  combine |= right;

  output = P4(combine);

  return output;
}

int encrypt(int input, vector<int> keys) {
  int output = 0;
  int tmp_left;
  output = IP(input);

  // split into 4bits
  int left = (0b11110000 & output)>>4;
  int right = 0b00001111 & output;

  left ^= fBox(right,keys.at(0));

  // swap
  tmp_left = left;
  left = right;
  right = tmp_left;

  left ^= fBox(right,keys.at(1));

  output = left<<4;
  output |= right;

  output = IPi(output);

  return output;
}

int decrypt(int input, vector<int> keys) {
  int output = 0;
  int tmp_left;
  output = IP(input);

  // split into 4bits
  int left = (0b11110000 & output)>>4;
  int right = 0b00001111 & output;

  left ^= fBox(right,keys.at(1));

  // swap
  tmp_left = left;
  left = right;
  right = tmp_left;

  left ^= fBox(right,keys.at(0));

  output = left<<4;
  output |= right;

  output = IPi(output);

  return output;
}

int tripleSDESenc(int msg, int key1,int key2) {
  vector<int> keys1 = gen_keys(key1);
  vector<int> keys2 = gen_keys(key2);

  int output = 0;
  output = encrypt(decrypt(encrypt(msg,keys1),keys2),keys1);
  return output;
}

int tripleSDESdec(int msg, int key1,int key2) {
  vector<int> keys1 = gen_keys(key1);
  vector<int> keys2 = gen_keys(key2);
  int output = 0;
  output = decrypt(encrypt(decrypt(msg,keys1),keys2),keys1);
  return output;
}

bool check_char(int bits){
  if (bits < 123 && bits > 96) {
    return true;
  }
  if (bits < 91 && bits > 64) {
    return true;
  }
  return false;
}

bool check_msb2(int bits){
  bits &= 0b11000000;
  bits >>= 6;
  bits &= 0b00000011;
  if (bits == 0b0000000001) {
    return true;
  }
  return false;
}

char * read_to_mem(string file) {
  std::ifstream cxt (file, std::ifstream::binary);
  char * buffer;
  if (cxt) {
    // get length of file:
    cxt.seekg (0, cxt.end);
    int length = cxt.tellg();
    cxt.seekg (0, cxt.beg);

    // allocate memory:
    buffer = new char [length];

    // read data as a block:
    cxt.read (buffer,length);

    cxt.close();
  }

  return buffer;
}

int brutx_SDES(char* buffer) {

  //char * buffer = read_to_mem(cxt);

  int buff[60];
  for (size_t j = 0; j < 60; j++) {
    buff[j] = 0;
    for (size_t i = 0; i < 8; i++) {
      buff[j] = (buff[j]<<1) | ((int)buffer[(j*8)+i]-48);
    }
  }
  vector<int> keys;
  int parenk_key;
  vector<int> dec;
  int dec_block;

  int c_key = -1; // return key
  for (size_t k = 0; k < 1024; k++) {
    keys = gen_keys(k);
    for (auto&block:buff) {
      dec_block = decrypt(block, keys);
      if (check_char(dec_block)) {
          dec.push_back(dec_block);
          if (dec.size() == 60) {
            c_key = k;
            goto stop;
          }
      } else {
          dec.clear();
          break;
      }
    }
  }
  stop:

  return c_key;
}

vector<int> brutx_T2SDES(char* buffer,int from = 0,int to = 1024) {

  int buff[60];
  for (size_t j = 0; j < 60; j++) {
    buff[j] = 0;
    for (size_t i = 0; i < 8; i++) {
      buff[j] = (buff[j]<<1) | ((int)buffer[(j*8)+i]-48);
    }
  }

  vector<int> keys;
  vector<int> dec;
  int dec_block;

  vector<int> c_keys; // return key

  for (size_t i = from; i < to; i++) {

    for (size_t k = 0; k < 1024; k++) {

      for (auto&block:buff) {
        dec_block = tripleSDESdec(block,i,k);
        if (check_char(dec_block)) {
          dec.push_back(dec_block);
          if (dec.size() == 60) {
            c_keys.push_back(i);
            c_keys.push_back(k);
            goto stop;
          }
        } else {
          dec.clear();
          break;
        }
      }
    }
  }
  stop:
  if (c_keys.size() < 1) {
    c_keys.push_back(-1);
    c_keys.push_back(-1);
    return c_keys;
  }
  return c_keys;
}

void print_plain(char* cxt, int key, int key2=0,bool triple=false) {
  int buff[60];
  for (size_t j = 0; j < 60; j++) {
    buff[j] = 0;
    for (size_t i = 0; i < 8; i++) {
      buff[j] = (buff[j]<<1) | ((int)cxt[(j*8)+i]-48);
    }
  }
  if (triple) {
    for (auto&block:buff) {
      cout << (char)tripleSDESdec(block,key,key2);
    }
    cout << endl;
  } else {
    for (auto&block:buff) {
      cout << (char)decrypt(block,gen_keys(key));
    }
    cout << endl;
  }
}

void T_brutx_T2SDES(char* cxt, vector<int>& T_ks,int f, int t, string msg) {
  vector<int> tmp = brutx_T2SDES(cxt,f,t);
  if (tmp[0] > 0) {
    T_ks = tmp;
  }
  cout << "Thread " << msg << " completed" << endl;
  ready = true;
}



int main() {
  const string cxt1f("cxt1.txt");
  const string cxt2f("cxt2.txt");

  clock_t beginLoad1 = clock();
  char* cxt1 = read_to_mem(cxt1f);
  char* cxt2 = read_to_mem(cxt2f);

  // Simplified DES - SDES
  clock_t beginSDES = clock();
  int ks = brutx_SDES(cxt1);
  if (ks<0) {
    cout << "Key could not be found" << endl;
  }
  cout << "K1:"<<ks << endl;
  print_plain(cxt1,ks);
  clock_t endSDES = clock();
  cout << "Time   SDES: " << float(endSDES-beginSDES)/CLOCKS_PER_SEC << " sek" << endl;
  cout << endl;

  // Triple SDES T2SDES
  clock_t beginT2SDES = clock();
  vector<int> kt = brutx_T2SDES(cxt2);
  if (kt[0]<0) {
    cout << "Key could not be found" << endl;
  }
  cout << "K1:"<<kt[0] << endl;
  cout << "K2:"<<kt[1] << endl;
  print_plain(cxt2,kt[0],kt[1],true);
  clock_t endT2SDES = clock();
  cout << "Time T2SDES: " << float(endT2SDES-beginT2SDES)/CLOCKS_PER_SEC << " sek" <<endl;
  cout << endl<<"TOTAL  TIME: " << float(endT2SDES-beginLoad1)/CLOCKS_PER_SEC << " sek" <<endl;

  /*
  clock_t timeThread = clock();
  vector<int> T_keys;
  std::thread t1 (T_brutx_T2SDES,cxt2,std::ref(T_keys),0,256,"t1");
  std::thread t2 (T_brutx_T2SDES,cxt2,std::ref(T_keys),256,512,"t2");
  std::thread t3 (T_brutx_T2SDES,cxt2,std::ref(T_keys),512,768,"t3");
  std::thread t4 (T_brutx_T2SDES,cxt2,std::ref(T_keys),1002,1024,"t4");
  t1.join();
  t2.join();
  t3.join();
  t4.join();

  std::unique_lock<std::mutex> lck(mtx);
  while (!ready) cv.wait(lck);
  cout << endl << endl;
  cout << T_keys[0] << endl;
  cout << T_keys[1] << endl;
  clock_t stopThread = clock();

  cout << "Time THREADS TIME: " << float(stopThread-timeThread)/CLOCKS_PER_SEC << endl;
  */


  //cout << bitset<8>(tripleSDESenc(0b00000000,0b0000000000,0b0000000000)) << endl;
  //cout << bitset<8>(tripleSDESenc(0b11010111,0b1000101110,0b0110101110)) << endl;
  //cout << bitset<8>(tripleSDESenc(0b10101010,0b1000101110,0b0110101110)) << endl;
  //cout << bitset<8>(tripleSDESenc(0b10101010,0b1111111111,0b1111111111)) << endl;
  //cout << bitset<8>(tripleSDESdec(0b11100110,0b1000101110,0b0110101110)) << endl;
  //cout << bitset<8>(tripleSDESdec(0b01010000,0b1011101111,0b0110101110)) << endl;
  //cout << bitset<8>(tripleSDESdec(0b10000000,0b0000000000,0b0000000000)) << endl;

  //cout << bitset<8>(a) << endl;
  //cout << bitset<8>(b) << endl;
  //cout << bitset<8>(c) << endl;
  //cout << endl;
  //cout << endl;

  //vector<int> super = gen_keys(1002);
  //cout <<  super.at(0) << endl;
  //cout << super.at(0) << endl;

  cout << endl;

  return 0;
}
