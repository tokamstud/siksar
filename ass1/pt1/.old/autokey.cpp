#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

struct ngram_decr {
  string ngram; // common word as key
  vector<string> pptext; // possible plain text output
  vector<string> promising_sets; // choosen promising word chunks
};

inline int pmod(int i, int n) {
    return (i % n + n) % n;
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

// Depricated seq
string seq(vector<char> vek, int a, int b) {
  vector<char> v;
  for (size_t i = a; i < b; i++) {
    /* code */
    v.push_back(vek[i]);
  }
  string str(v.begin(),v.end());
  return str;
}

// to_possible_fragments decrypts partial message in scope with given key
vector<string> to_possible_fragments(vector<char> vek, string word, int scope) {
  vector<char> key(word.begin(), word.end());
  vector<string> pptext;
  // TODO: go through vector and decipher mesasge with ::word
  // save decrypted message in vector
  for (size_t n = 0; n < key.size(); n++) {
    vector<char> decText;
    for (size_t j = 0; j < n; j++) {
      decText.push_back('*');
    }
    for (size_t i = 0; i < scope; i++) {
      int c = vek[i]-65;
      int k = (key[pmod(i,key.size())])-65;
      char p = (pmod((c-k),26))+65;
      decText.push_back(p);
    }
    string str(decText.begin(), decText.end());
    pptext.push_back(str);
  }
  return pptext;
}

// format_vector formats given string to fragments separated by space
string format_vector(string str, int space) {
  vector<char> vec(str.begin(), str.end());
  string pplaintext;
  for (size_t i = 0; i < vec.size(); i++) {
    if (i != 0 && i%space == 0) {
      pplaintext = pplaintext + " " + vec[i];
    } else {
      pplaintext += vec[i];
    }
  }
  pplaintext += "\n";
  return pplaintext;
}

string generate_plaintext_fragments(vector<string> ppvector, string ngram) {
  const string pptffolder = "./pptfrags/";
  string pptfrag;
  for (size_t i = 0; i < ppvector.size(); i++) {
    pptfrag += format_vector(ppvector[i], ngram.length());
  }
  // TODO: export to one file

  //string path = pptffolder + ngram;
  //ofstream outfile(path.c_str());
  string data(pptfrag);
  //outfile << data << endl;
  //outfile.close();
  return data +'\n';
}

void run_decription_sequence(string ctfile,vector<string> ngrams,int workspace=415) {
  ngram_decr ngd[64];

  string fragments;

  vector<char> ciphertext = read_to_vector_ns(ctfile);
  for (size_t i = 0; i < ngrams.size(); i++) {
    ngd[i].ngram = ngrams[i];
    ngd[i].pptext = to_possible_fragments(ciphertext,ngd[i].ngram,workspace);
    fragments += generate_plaintext_fragments(ngd[i].pptext,ngd[i].ngram);
  }

  string path = "./pptfrags/plaintext.frag";
  ofstream outfile(path.c_str());
  string data(fragments);
  outfile << data << endl;
  outfile.close();
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

void filter_fragments(string pffile) {

}


int main() {
  const int workspace = 20;
  const string ctfile("ciphertext");
  const string ngfile("./pptfrags/ngrams"); // file with most frequent n-grams
  const string pffile("./pptfrags/plaintext.frag");
  // clear folder
  system("exec rm pptfrags/*.frag");

  vector<string> ngrams = get_ngrams(ngfile);

  run_decription_sequence(ctfile,ngrams,workspace);
  filter_fragments(pffile);


  return 0;
}
