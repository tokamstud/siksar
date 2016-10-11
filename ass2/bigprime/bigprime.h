#ifndef BIG_PRIME_H
#define BIG_PRIME_H

class Bigprime{
public:
	mpz_t prime;
	mpz_t big_odd;
	mpz_t big_rand;
	unsigned long int power;
	gmp_randstate_t state;

	Bigprime(unsigned long int power);
	void initialize_random();
	void generate_random(int i);
	void generate_prime();
	void set_new_prime();
};

#endif
