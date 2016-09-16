#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<algorithm>
#include<unistd.h>

using namespace std;

struct ngram_decr {
  string ngram; // common word as key
  vector<string> ppttext; // possible plain text output
  vector<string> promising_sets; // choosen promising word chunks
};

void str_to_file(string fragments, string ppfile) {
  ofstream outfile(ppfile.c_str());
  string data(fragments);
  outfile << data << endl;
  outfile.close();
}

inline int pmod(int i, int n) {
    return (i % n + n) % n;
}
string vector_to_string(vector<char> vec) {
  string str;
  for (size_t i = 0; i < vec.size(); i++) {
    str += vec.at(i);
  }
  return str;
}

string str_vector_to_string(vector<string> vec) {
  string str;
  for (size_t i = 0; i < vec.size(); i++) {
    str = str + " " + vec.at(i);
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
    if (vec.at(i) == ' ' || vec.at(i) == '\n') {
      continue;
    }
      noSpaceVec.push_back(vec.at(i));
  }
  return noSpaceVec;
}

// read_to_vector_ns returns vector<char> of the given file name with no spaces or nl
vector<char> read_to_vector_ns(string filename) {
  return remove_spaces(read_to_vector(filename));
}


// decrypt_with_key decrypts partial message in scope with given key
vector<char> decrypt_with_key(vector<char> vek, int scope, vector<char> key) {

  // save decrypted message in vector
  vector<char> decText;
  for (size_t i = 0; i < key.size(); i++) {
    int c = vek.at(i)-65;
    int k = (key.at(pmod(i,key.size())))-65;
    char p = (pmod((c-k),26))+65;
    decText.push_back(p);
  }
  for (size_t j = 0; j < scope-key.size(); j++) {
    int c = vek.at(j+key.size())-65;
    int k = decText.at(j)-65;
    char p = (pmod((c-k),26))+65;
    decText.push_back(p);
  }
  return decText;
}


vector<string> split_to_ngrams(vector<char> vec, int n) {
  vector<string> ngrams;
  vector<char> part;
  int pass_count = 0;

  for (size_t i = 0; i < vec.size(); i++) {
    if (i%n == 0 && i != 0) {
      ngrams.push_back(vector_to_string(part));
      part.clear();
    }
    int idx = i+pass_count;
    if (!(idx >= vec.size())) {
      part.push_back(vec.at(idx));
    }
    if ((i+1) == vec.size() && pass_count < n-1) {
      i = -1;
      pass_count++;
      part.clear();
    }
  }

  return ngrams;
}

vector<char> prepare_relevant_chunks(vector<char> plainvec, int keyl, int part, vector<char> v) {
  vector<char> relevant_chunks;
  vector<char> kschunk;
  vector<char> pschunk;

  for (size_t i = 0; i < plainvec.size(); i++) {
    if (i%keyl == 0 && i != 0) {
      for (size_t j = 0; j < part; j++) {
        pschunk.push_back(kschunk.at(j));
      }
      for (auto & e : pschunk) {
        relevant_chunks.push_back(e);
      }
      pschunk.clear();
      kschunk.clear();
    }
    kschunk.push_back(plainvec.at(i));
  }

  return relevant_chunks;
}

long quad_stat(vector<string> data, string sample) {
  // TODO: quadram statistics here

  return 5;
}

long log_prob(vector<char> plainvec, int keyl, int part, vector<char> v,string sample) {

  vector<char> relevant_chunks = prepare_relevant_chunks(plainvec,keyl,part,v);
  vector<string> data = split_to_ngrams(relevant_chunks,4);
  long prop = quad_stat(data,sample);

  return prop;
}


map<vector<char>,long> decryption_sequence(vector<char> cipvec,string sample, int scope,string output) {
  vector<char> plainvector;
  long maxscore = 0;
  long score = 0;
  int pass_count = 1;
  long pass_max = 0;

  map<vector<char>,long> parent_score;
  map<vector<char>,long> tmp_parent_score;

  vector<char> init_key (1,'A');
  vector<char> parent_key;
  vector<char> child_key;
  for (size_t i = 2; i < 7; i++) {
    // next independent session from previous
    init_key.push_back('A');
    parent_key = init_key;

    for (size_t j = 0; j < i; j++) {
      child_key = parent_key;
      for (size_t k = 0; k < 26; k++) {

        child_key.at(j) = 'A' + k;

        plainvector = decrypt_with_key(cipvec, scope, child_key);
        if (pass_count > 1) {
          score = log_prob(plainvector,i,i,child_key,sample);
        } else {
          score = log_prob(plainvector, i,j,child_key,sample);
        }

        if (score > maxscore) {
          maxscore = score;
          parent_key = child_key;
        }

      }
      if ((j+1) == i) {
        // if this was last iteration
        if (pass_count == 1) {
          j = 0;
          pass_count++;
          pass_max = maxscore;
        }
        if (pass_count > 1 && maxscore > pass_max) {
          j = 0;
          pass_max = maxscore;
          pass_count++;
        }
      }
    }
    // saved this key value to public var
    parent_score[parent_key] = maxscore;

    maxscore = 0;
  }
  string str;
  for (auto& x: parent_score) {
    string st = vector_to_string(x.first);
    str+=st + ": " + to_string(x.second) + "\n";
  }
  str_to_file(str,output);
  return parent_score;
}

int main() {
  const int workspace = 415;
  const string ctfile("ciphertext");
  const string output("./output/plaintext.frag");
  const string sample("./ngrams/quadgrams");
  // clear folder
  system("exec rm output/*.frag");

  vector<char> ciphervector = read_to_vector_ns(ctfile);
  // vector<char> plaintvector = decrypt_with_key(ciphervector,workspace, {'D','A','T','F','B','A'});
  //string fragments = vector_to_string(plaintvector);
  //str_to_file(fragments,output);

  map<vector<char>, long> result = decryption_sequence(ciphervector,sample,workspace,output);


  return 0;
}