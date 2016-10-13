#include <iostream>
#include <math.h>
#include <gmp.h>

#include "../bigprime/bigprime.h"
#include "client.h"

#include "aes.h"

using namespace std;

void encrypt(int key) {
	byte iv[AES::BLOCKSIZE];
	rnd.GenerateBlock(iv, AES::BLOCKSIZE);

	char plainText[] = "Hello Bobby!";
	int messageLen = (int)strlen(plainText) + 1;


	// Encryption
	CFV_Mode<AES>::Encryption cfbEncryption(key, sizeof(key), iv);
	cfvEncryption.ProsessData((byte*)plainText, (byte*)plainText, messageLen);

	// Decrypt
	CFV_Mode<AES>::Decryption cfbDecryption(key, sizeof(key), iv);
	cfbDecryption.ProsessData((byte*)plainText, (byte*)plainText, messageLen);

}

int main(int argc, char* argv[]) {
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long int rseed = time(NULL);
	gmp_randseed_ui(state, rseed);

	// generate seed_prime for Prime and generator
	Bigprime seed (2048);

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
	alice.print_secret();//print
	bobby.print_secret();

	// share BBS values
	alice.find_bbs_init();
	mpz_t alice_n;mpz_init(alice_n);
	alice.get_bbs_init(alice_n);
	bobby.set_bbs_init(alice_n); // set the same n value as alice has for Blum Blum Shub

	alice.generate_next_key();
	bobby.generate_next_key();

	alice.generate_next_key();
	bobby.generate_next_key();

	return 0;
}
