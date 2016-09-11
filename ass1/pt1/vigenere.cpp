#include<iostream>
#include<fstream>
#include<vector>
#include<typeinfo>

using namespace std;

// decrypt returns decrypted text with given key
string decrypt(vector<char> ciphertext, string key) {
  // TODO:
  string plaintext;
  plaintext = "DECRYPTEDTEXT";
  return plaintext;
}

// removeSpaces removes spaces from encrypted text as vector<char>
vector<char> removeSpaces(vector<char> vec) {
  vector<char> noSpaceVec;
  int j = 0;
  for (size_t i = 0; i < vec.size(); i++) {
    if (vec[i] == ' ' || vec[i] == '\n') {
      continue;
    }
      noSpaceVec.push_back(vec[i]);
  }
  return noSpaceVec;
}

string seq(vector<char> vek, int a, int b) {
  vector<char> v;
  for (size_t i = a; i < b; i++) {
    /* code */
    v.push_back(vek[i]);
  }
  string str(v.begin(),v.end());
  return str;
}
// findRepeatingSequence returns array of repeating sequences and it's displacement
int findRepeatingSequence(vector<char> cipherVector) {
  // TODO:
  vector<char> ct = removeSpaces(cipherVector);
  int maxSeq = (ct.size()/2);
  int ctlen = cipherVector.size();

  string currSeq;
  string compSeq;


  // current sequence size
  int css = 2;
  // current sequence size loop
  for (size_t k = 0; k < maxSeq; k++) {

    for (size_t cs = 0;;cs++) {
      // how many compares
      int from = cs;
      int to = from+css;
      int from2 = to;
      int to2 = from2+css;


      if (to > ct.size()) {
        css++;
        break;
        /* code */
      }

      for (size_t i = cs+1; i < ctlen-cs; i++) {
        currSeq = seq(ct,from,to);
        compSeq = seq(ct,from2++,to2++);
        //cout << " this: " + currSeq;
        //cout << "\n that: " + compSeq << endl;
        if (currSeq.compare(compSeq) == 0) {
          cout << currSeq << endl;

          /* code */
        }
        if (to2 > ct.size()) {
          break;
        }
      }
    }
  }

  return 0;
}

// keySizeSearch returns possible keysizes in array
int keySizeSearch(vector<char> ciphertext) {
  // TODO:
  return 0;
}

// printVector prints the given vector to console including newlines
void printVector(vector<char> vec) {
  for (size_t i = 0; i < vec.size(); i++) {
    cout << vec[i];
  }
}

// readToVector returns vector<char> of the given file name
vector<char> readToVector(string filename) {
  ifstream ifs(filename, ios::binary);

  ifs.seekg(0, ios::end);
  ifstream::pos_type filesize = ifs.tellg();
  ifs.seekg(0, ios::beg);

  vector<char> bytes(filesize);

  ifs.read(&bytes[0], filesize);

  return bytes;
}
int main() {

  string cipherfile = "ciphertext";
  vector<char> cipherVector = readToVector(cipherfile);

  findRepeatingSequence(cipherVector);

  return 0;
}
