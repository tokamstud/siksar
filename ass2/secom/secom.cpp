#include <iostream>
#include <math.h>
#include <gmp.h>

#include "../bigprime/bigprime.h"
#include "client.h"

using namespace std;

int main(int argc, char* argv[]) {
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long int rseed = time(NULL);
	gmp_randseed_ui(state, rseed);

	// generate seed_prime for Prime and generator
	Bigprime seed (1024);

	// alice proposes Prime and alpha
	Client alice (seed.prime, state);

	//agreed Prime and generator/alpha
	mpz_t Prime,alpha;
	mpz_init(Prime);
	mpz_init(alpha);
	mpz_set(Prime,alice.Prime);
	mpz_set(alpha,alice.alpha);

	// bobby initializes Prime and alpha that are publicly available
	Client bobby (Prime,alpha, state);

	//public key exchange
	alice.exchange_public(bobby.Pu_k);
	bobby.exchange_public(alice.Pu_k);

	alice.generate_secret_key();
	bobby.generate_secret_key();

	alice.print_secret();
	bobby.print_secret();

	alice.generate_next_key();
	bobby.generate_next_key();

	cout<<endl<<endl;
	alice.print_nsecret();
	bobby.print_nsecret();


	return 0;
}
