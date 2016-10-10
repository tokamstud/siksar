#include<iostream>
#include<math.h>
#include<gmp.h>
#include<time.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <time.h>

#include "bigprime.h"

using namespace std;

Bigprime::Bigprime(unsigned long int power) {
	this->power = power;
	mpz_init(this->prime);
	mpz_init(this->big_odd);
	mpz_init(this->big_rand);
	/*
	initializing random seed in constructor,
	otherwise it might generate same sequence if time() runs too often.
	*/
	initialize_random();

	generate_prime();
}

void Bigprime::initialize_random() {
	// sets the seed for random number as current time;
	gmp_randinit_mt(this->state);
	unsigned long int seed = time(NULL);
	gmp_randseed_ui(this->state, seed);
}

// generates random number if i is 1 generates odd random,
// if i is 0 generates random < big_odd-2
void Bigprime::generate_random(int i) {
	mpz_t random;mpz_init(random);

	mpz_t mod,c;
	mpz_init(mod);
	mpz_init(c);
	mpz_set_str(mod,"2",10);
	mpz_set_str(c,"0",10);

	if(i) {
		int test = 1;
		while(test) {
			mpz_urandomb(random, this->state, this->power);
			if(mpz_congruent_p(random,c,mod)==0) {
				mpz_set(this->big_odd,random);
				test=0;
		  }
		}
	} else {
		mpz_t c;mpz_init(c);mpz_set_str(c,"1",10);
		mpz_t odd_2;mpz_init(odd_2);mpz_set(odd_2,this->big_odd);
		mpz_sub(odd_2,odd_2,c);
		mpz_urandomm(random, this->state, odd_2);
		mpz_set(this->big_rand,random);
	}
}

void Bigprime::generate_prime() {
	mpz_t test;mpz_init(test);
	generate_random(1);//odd
	mpz_t c;mpz_init(c);mpz_set_str(c,"1",10);
	mpz_t odd_1;mpz_init(odd_1);mpz_set(odd_1,this->big_odd);
	mpz_sub(odd_1,odd_1,c);
	// fermats prime test 50 times
	for (size_t i = 0; i < 50; i++) {
		generate_random(0);//radnom < big_odd-2
		// a^(p-1) mod p
		mpz_powm(test,this->big_rand,odd_1,this->big_odd);
		// is congruent to 1?
		if (mpz_cmp(test,c)!=0) {
			generate_prime();
			return;
		}
	}
  mpz_set(this->prime,this->big_odd);
}
