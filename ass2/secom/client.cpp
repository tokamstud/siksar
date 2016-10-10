#include<math.h>
#include<gmp.h>

#include "client.h"

Client::Client(mpz_t Prime, mpz_t alpha) {
	mpz_init(this->Pu_x);
	mpz_init(this->Pu_k);
	mpz_init(this->Prime);
	mpz_init(this->alpha);

	generate_private_key(Prime, alpha);
}

void Client::generate_private_key(mpz_t Prime, mpz_t alpha) {
	// TODO: set public key based on cyclic group, Prime is ok
	// random < Prime
}

void Client::generate_secret_key() {
	// TODO: the secret key = Pu_x ^ Pr_k mod Prime
}

void Client::get_public(mpz_t ret) {
	//takes parameter to store the public number in, saves it
	mpz_init(ret);
	mpz_set(ret,this->Pu_k);
}

void Client::set_other_public(mpz_t Pu_x) {
	mpz_set(this->Pu_x, Pu_x);
}
