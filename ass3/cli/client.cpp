#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <gmp.h>

#include "client.h"
#include "../hash/hash.h"


using namespace std;

Client::Client(int id) {
	this->cli_id = id;
	mpz_init(this->p);
	mpz_init(this->q);
	mpz_init(this->g);
	mpz_init(this->x);
	mpz_init(this->public_key);
	mpz_init(this->pmsn);

	mpz_init(this->pmsn);

}
//Client::~Client() {}

void Client::gen_private() {
	gmp_randstate_t state;gmp_randinit_mt(state);
	unsigned long int seed = time(NULL);
	gmp_randseed_ui(state, seed);

	mpz_urandomm (this->x, state, this->q);
}

void Client::gen_public() {
	// pub_k = g^x mod p
	mpz_powm(this->public_key, this->g, this->x, this->p);
}

// save message
void Client::save_msg(string msg) {
	this->msg_out = msg;
}

void Client::hash_msg() {
	// TODO: use hash/hash.h header file include
	// calculate hash and set its value to mpz_t hash

	mpz_init_set_str (this->hash_of_msg, "0", 10);
}
// sets signature
void Client::sign() {
	gmp_randstate_t state;gmp_randinit_mt(state);
	unsigned long int seed = time(NULL);
	gmp_randseed_ui(state, seed);
	// setting user per message secret number
	mpz_urandomm (this->pmsn, state, this->q);

	// encrpt (this->hash_of_msg) with private_key
	mpz_init(sign_r);
	mpz_init(sign_s);
	// signgning
	// r = (g^k mod p) mod q
	mpz_powm(this->sign_r, this->g, this->pmsn, this->p);
	mpz_powm_ui(this->sign_r, this->sign_r, 1, this->q);

	// k⁻¹
	mpz_t inv_pmsn; mpz_init(inv_pmsn);
	mpz_invert (inv_pmsn, this->pmsn, this->q);

	mpz_init(this->hash_of_msg);

	std::string str;
	str = sha512(this->msg_out);
	const char * c = str.c_str();
	mpz_set_str(this->hash_of_msg,c,16);
	cout << endl;
	cout << "hash of the message that will be used for signing: " << endl;
	cout << str << endl << endl;

	// calculating s = {k⁻1 (H(M) + xr)} mod q
	mpz_t intermidiet; mpz_init(intermidiet);
	mpz_mul(intermidiet, this->x, this->sign_r);

	mpz_add(this->sign_s, this->hash_of_msg, intermidiet);
	mpz_mod (this->sign_s, this->sign_s, this->q);

	// signature is now this->(sign_r, sign_s);

}

// sets msg_out
void Client::send_message(Client *cli) {
	cli->set_msg_in(this->msg_out);
	// sending signature...
	mpz_init_set (cli->in_sign_r, this->sign_r);
	mpz_init_set (cli->in_sign_s, this->sign_s);
}

bool Client::verify_message(Client *cli) {
	// TODO:
	// set signature_verification = H(msg_in)
	// compare D(signature_in) == signature_verification
	// in short if
	//			D(signature_in) == H(msg_in) then pass
	// test if this->in_sign_r == sign_r_ver
	mpz_init(this->sign_r_ver);
	mpz_t w; mpz_init(w);

	mpz_t inv_in_sign_s; mpz_init(inv_in_sign_s);
	mpz_invert (inv_in_sign_s, this->in_sign_s, this->q);

	mpz_mod(w, inv_in_sign_s, this->q);

	mpz_t u1, u2; mpz_init(u1); mpz_init(u2);

	std::string str;
	str = sha512(this->msg_in);
	const char * c = str.c_str();
	mpz_set_str(this->hash_of_msg,c,16);

	mpz_mul(u1, this->hash_of_msg, w);
	mpz_mod(u1, u1, this->q);

	mpz_mul(u2, u2, w);
	mpz_mod(u2, u2, this->q);

	mpz_t g_u1, y_u2, v; mpz_init(g_u1); mpz_init(y_u2); mpz_init(v);
	//(A * B) mod C = (A mod C * B mod C) mod C
	mpz_powm(g_u1, this->g, u1, this->p);
	mpz_powm(y_u2, cli->public_key, u2, this->p);
	mpz_mul(v, g_u1, y_u2);
	mpz_mod(v, v, this->p);
	mpz_mod(v, v, this->q);

	cout << "Verifying that the checksum is correct : " << endl;
	mpz_out_str(stdout,16,cli->sign_r);cout<<endl;
	mpz_out_str(stdout,16,this->in_sign_r);cout<<endl;

	// if v == this->in_sign_r then pass
	if (mpz_cmp(cli->sign_r, this->in_sign_r) == 0) {
		return true;
	} else {
		return false;
	}

}

void Client::set_msg_in(string msg) {
	this->msg_in = msg;
}
