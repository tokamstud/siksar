#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <iostream>
#include <iomanip>

#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"

using namespace std;

class Client {
public:
	string client_name;
	mpz_t Pu_x;//public key of recipient
	mpz_t Pu_k;//public own key
	mpz_t Prime;
	mpz_t alpha;//primitive root
	mpz_t n;//blum blum shub n=modulo
	Client(mpz_t Prime, mpz_t alpha,gmp_randstate_t state, string name);
	Client(mpz_t seed_prime,gmp_randstate_t state, string name);
	~Client();
	void exchange_public(mpz_t Pu_x);
	void calc_public(mpz_t ret);
	// Public agreement;
	void make_alpha_Prime(mpz_t prime_seed);
	void print_secret();
	void print_nsecret();
	void find_bbs_init();
	void set_bbs_init(mpz_t n);
	void get_bbs_init(mpz_t n);
	mpz_t Pr_k;//Private key
	mpz_t Se_k; // Secret key for enc/dec
	mpz_t Se_kn;// next key
	void generate_private_key(mpz_t Prime, mpz_t alpha, gmp_randstate_t state);
	void generate_secret_key();

	void generate_next_key();

	// enc-dec
	string encrypt(string plaintext);
	void decrypt(string ciphertext);
	int size_p;
};

#endif
