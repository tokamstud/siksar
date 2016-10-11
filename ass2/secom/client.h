#ifndef CLIENT_H
#define CLIENT_H

class Client {
public:
	mpz_t Pu_x;//public key of recipient
	mpz_t Pu_k;//public own key
	mpz_t Prime;
	mpz_t alpha;//primitive root
	Client(mpz_t Prime, mpz_t alpha,gmp_randstate_t state);
	Client(mpz_t seed_prime,gmp_randstate_t state);
	~Client();
	void exchange_public(mpz_t Pu_x);
	void calc_public(mpz_t ret);
	// Public agreement;
	void make_alpha_Prime(mpz_t prime_seed);
	void print_secret();
	void print_nsecret();

	mpz_t Pr_k;//Private key
	mpz_t Se_k; // Secret key for enc/dec
	mpz_t Se_kn;// next key
	void generate_private_key(mpz_t Prime, mpz_t alpha, gmp_randstate_t state);
	void generate_secret_key();

	void generate_next_key();
};

#endif
