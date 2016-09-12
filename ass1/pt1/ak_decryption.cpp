#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>
#include <unistd.h>

using namespace std;

struct ngram_decr {
  string ngram; // common word as key
  vector<string> pptext; // possible plain text output
  vector<string> promising_sets; // choosen promising word chunks
};
inline int pmod(int i, int n) {
    return (i % n + n) % n;
}
string vector_to_string(vector<char> vec) {
  string str;
  for (size_t i = 0; i < vec.size(); i++) {
    str += vec[i];
  }
  return str;
}

vector<string> get_ngrams(string path) {
  vector<string> ngrams;
  ifstream file(path);
  string ngram;
  while (file >> ngram) {
      ngrams.push_back(ngram);
  }
  return ngrams;
}
// read_to_vector returns vector<char> of the given file name
vector<char> read_to_vector(string filename) {
  ifstream ifs(filename, ios::binary);

  ifs.seekg(0, ios::end);
  ifstream::pos_type filesize = ifs.tellg();
  ifs.seekg(0, ios::beg);

  vector<char> bytes(filesize);

  ifs.read(&bytes[0], filesize);

  return bytes;
}
// remove_spaces removes spaces from encrypted text as vector<char>
vector<char> remove_spaces(vector<char> vec) {
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

// read_to_vector_ns returns vector<char> of the given file name with no spaces or nl
vector<char> read_to_vector_ns(string filename) {
  return remove_spaces(read_to_vector(filename));
}

// decrypt_with_key decrypts partial message in scope with given key
vector<char> decrypt_with_key(vector<char> vek, int scope, string keystr="DATFBA") {
  usleep(4000000);

  vector<char> key(keystr.begin(), keystr.end());
  // TODO: go through vector and decipher mesasge with keystr
  // save decrypted message in vector
  vector<char> decText;
  for (size_t i = 0; i < key.size(); i++) {
    int c = vek[i]-65;
    int k = (key[pmod(i,key.size())])-65;
    char p = (pmod((c-k),26))+65;
    decText.push_back(p);
  }
  for (size_t j = 0; j < scope; j++) {
    int c = vek[j+key.size()]-65;
    int k = decText[j]-65;
    char p = (pmod((c-k),26))+65;
    decText.push_back(p);
  }
  return decText;
}

long log_prob(vector<char> cipvec, vector<char> child_key) {
  // TODO: count the log. probability based on quadgram statistics
  // compare only the length of scrambeled child_key part "XYZAAA"

  return 1;
}

string generate_key(vector<char> ck, int length, int position, int increment) {
  ck[position] = (ck[position] + increment) % 26;

  return "0";
}

void decryption_sequence(vector<char> cipvec, int scope) {
  vector<char> plainvector;
  vector<int> scores;

  map<long, vector<char>> current_parent;

  make_heap (plainvector.begin(), plainvector.end());

  for (size_t i = 2; i < 6; i++) {
    for (size_t j = 0; j < i; j++) {
      for (size_t k = 0; k < 26; k++) {
        string child_key = generate_key(current_parent[scores[0]],i,j,k);

        vector<char> key(child_key.begin(), child_key.end());
        plainvector = decrypt_with_key(key, scope);

        long score = log_prob(plainvector, key);
        scores.push_back(score);
        if (score == scores[0]) {
          current_parent[score] = plainvector;
          scores.erase (scores.begin(), scores.end());
        }
      }
    }
  }
}

int main() {
  const int workspace = 415;
  const string ctfile("ciphertext");
  const string ppfile("./pptext/plaintext.frag");
  // clear folder
  system("exec rm pptext/*.frag");

  vector<char> ciphervector = read_to_vector_ns(ctfile);
  // TODO: make decryption sequence function based on quadgram statistics

  vector<char> plaintvector = decrypt_with_key(ciphervector,workspace);
  string fragments = vector_to_string(plaintvector);

  ofstream outfile(ppfile.c_str());
  string data(fragments);
  outfile << data << endl;
  outfile.close();

  return 0;
}
