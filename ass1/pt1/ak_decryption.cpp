#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <unistd.h>
#include <math.h>
#include <utility>
#include <numeric>

using namespace std;

struct ngram_decr {
  string ngram; // common word as key
  vector<string> ppttext; // possible plain text output
  vector<string> promising_sets; // choosen promising word chunks
};

map<string,long> quadgrams;


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

map<string,long> read_quadgrams(string sample) {
  ifstream infile(sample);

  map<string,long> qdg;
  string a;
  long b;
  while (infile >> a >> b){
    // process pair (a,b)
    qdg[a] = b;
  }
  return qdg;
}

map<string, float> quadg_logscore(map<string, long> qdg, float total) {
  map<string, float> nqdg;
  for (auto &ent : qdg) {
    nqdg[ent.first] = log10((float(ent.second)/total));
  }
  return nqdg;
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

int counters = 0;

vector<char> prepare_relevant_chunks(vector<char> plainvec, int keyl, int part) {
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

long quad_stat(vector<string> data, map<string,float> quadscore, long totaln) {
  // TODO: count probability of data
  float totalprob = 0;

  for (auto &quad : data) {
    if (quadscore.count(quad) > 0) {
      totalprob += quadscore[quad];
    } else {
      totalprob += log10(0.1/totaln);
    }
  }

  return totalprob;
}

long log_prob(vector<char> plainvec, int part, vector<char> key, map<string,float> quadscore, long totaln) {

  vector<char> relevant_chunks = prepare_relevant_chunks(plainvec,key.size(),part);
  vector<string> data = split_to_ngrams(relevant_chunks,4);
  long prob = quad_stat(data,quadscore, totaln);
  return prob;
}

map<vector<char>,long> decryption_sequence(vector<char> cipvec, int scope, string output, map<string,float> quadscore, long totaln) {

  vector<char> child_dec;
  long maxscore = -10000;
  long score = -10000;
  long last_rount_max = -10000;
  int pass_count = 1;
  bool is_whole_key = false;

  map<vector<char>,long> parent_score;
  map<vector<char>,long> tmp_parent_score;

  vector<char> init_key (1,'A');
  vector<char> parent_key;
  vector<char> child_key;

  int keylength;
  for (size_t i = 2; i < 7; i++) {
    // next independent session from previous
    init_key.push_back('A');
    parent_key = init_key;
    keylength = i;
    for (size_t j = 0; j < i; j++) {
      child_key = parent_key;
      for (size_t k = 0; k < 26; k++) {
        int offset;
        if (is_whole_key) {
          offset = keylength;
        } else {
          offset = j;
        }

        child_key.at(j) = 'A' + k;
        child_dec = decrypt_with_key(cipvec, scope, child_key);
        score = log_prob(child_dec,offset,child_key,quadscore, totaln);

        //cout << vector_to_string(child_key) << " : " << score << endl;
        if (score > maxscore) {
          maxscore = score;
          parent_key = child_key;
          //cout << vector_to_string(child_key) << " : " << score << endl;
        }

      }
      if ((j+1) == keylength) {
        // if this was last letter in current key
        is_whole_key = true;
        if (!(maxscore > last_rount_max)) {
          break;
        }
        j--; // star from f rst letter current key
        last_rount_max = maxscore;
        }
      }
      // saved this key value to public var
      parent_score[parent_key] = maxscore;
      last_rount_max = -1000;
      maxscore = -10000;
    }

  string str;
  for (auto& x: parent_score) {
    string st = vector_to_string(x.first);
    str+=st + ": " + to_string(x.second) + "\n";
    str+=vector_to_string(decrypt_with_key(cipvec,scope,x.first)) + "\n";
  }

  str_to_file(str,output);
  return parent_score;
}

int main() {
  const int workspace = 415;
  const string ctfile("ciphertext");
  const string output("./output/C++-results.md");
  const string sample("./ngrams/quadgrams");
  // clear folder
  //system("exec rm output/*.md");

  vector<char> ciphervector = read_to_vector_ns(ctfile);

  map<string, long> qdg = read_quadgrams(sample);
  float totn =0.0; // total number of quadgrams in sample
  for (auto &ent : qdg) {
    totn += ent.second;
  }
  map<string, float> qdglog = quadg_logscore(qdg, totn);
  map<vector<char>, long> result = decryption_sequence(ciphervector,workspace,output,qdglog,totn);

  // vector<char> plaintvector = decrypt_with_key(ciphervector,workspace, {'D','A','T','F','B','A'});
  //string fragments = vector_to_string(plaintvector);
  //str_to_file(fragments,output);

  return 0;
}
