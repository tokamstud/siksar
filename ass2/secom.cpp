#include <iostream>
#include <math.h>
#include <gmp.h>
#include "cryptopp/cryptlib.h"

// local
#include "bigprime/bigprime.h"
#include "client.h"



using namespace std;

void start_conversation(Client alice, Client bobby) {

		string ciphertext;
		string plaintext;
		// conversation start
		while(1) {

			// alice sends
			cout << "alice:";
			getline (cin,plaintext);
			alice.generate_next_key();
			ciphertext = alice.encrypt(plaintext);

			bobby.generate_next_key();
			bobby.decrypt(ciphertext);

			// bobby sends
			cout << "bobby:";
			getline (cin,plaintext);
			bobby.generate_next_key();
			ciphertext = bobby.encrypt(plaintext);

			alice.generate_next_key();
			alice.decrypt(ciphertext);
		}
}

void get_from_file(mpz_t rop) {
	FILE* pfile;
	pfile = fopen("./primes/*","r");
	mpz_inp_str(rop,pfile,62);
	fclose(pfile);
}

int main(int argc, char* argv[]) {
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long int rseed = time(NULL);
	gmp_randseed_ui(state, rseed);

	int psize;
	mpz_t seed_prime;
	mpz_init(seed_prime);

	if (argc < 2) {
		psize = 1024;
	} else {
		psize = atoi(argv[1]);
	}

	Bigprime seed (psize);
	mpz_set(seed_prime,seed.prime);

	// alice proposes Prime and alpha
	string name1 = "Alice";

	Client alice (seed_prime, state, name1);

	//agreed Prime and generator/alpha
	mpz_t Prime,alpha;
	mpz_init(Prime);
	mpz_init(alpha);
	mpz_set(Prime,alice.Prime);
	mpz_set(alpha,alice.alpha);

	cout << endl;
	cout << "The cyclic group and public parameters used" << endl;
	cout << "Cyclic group " << psize << "bit:" << endl;
	mpz_out_str(stdout,62,Prime);cout<<endl;
	cout << "The generator:" << endl;
	mpz_out_str(stdout,62,alpha);cout<<endl;

	// bobby initializes Prime and alpha that are publicly available
	string name2 = "Bobby";
	Client bobby (Prime,alpha, state, name2);

	//public key exchange (secure exchange)
	alice.exchange_public(bobby.Pu_k);
	bobby.exchange_public(alice.Pu_k);

	// alice and bob are generating the same secret key
	alice.generate_secret_key();
	bobby.generate_secret_key();

	cout<<endl;
	cout << "The shared seed key K_ab:" << endl;
	cout << "	Alice:" << endl;
	mpz_out_str(stdout,62,alice.Se_k);cout<<endl;
	cout << "	Bobby:" << endl;
	mpz_out_str(stdout,62,bobby.Se_k);cout<<endl;


	// share BBS values (assuming they share it securely, this is unsecure exchange!)
	alice.find_bbs_init();
	mpz_t alice_n;mpz_init(alice_n);
	alice.get_bbs_init(alice_n);
	bobby.set_bbs_init(alice_n); // set the same n value as alice has for Blum Blum Shub
	// alice and bob can generate the same Blum Blum Shub sequence based on the secret key
	alice.generate_next_key();
	bobby.generate_next_key();

	cout << endl;
	cout << "The shared key K for encryption:" << endl;
	cout << "	Alice:" << endl;
	mpz_out_str(stdout,62,alice.Se_kn);cout<<endl;
	cout << "	Bobby:" << endl;
	mpz_out_str(stdout,62,bobby.Se_kn);cout<<endl;
	cout << endl;

	start_conversation(alice, bobby);

	return 0;
}

void save_prime(mpz_t prime) {
	int psize = mpz_sizeinbase(prime,2);
	string path = static_cast<ostringstream*>(&(ostringstream() << "./primes/" << psize) )->str();
	FILE* pfile;
	pfile = fopen(path.c_str(),"w");
	mpz_out_str(pfile,62,prime);
	fclose(pfile);
}
