#ifndef CLIENT_H
#define CLIENT_H
#include <stdint.h>
#include <gmp.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

class Client {
public:
	int cli_id;

	// DSS variables
	mpz_t p, q, g, x, pmsn;
	// sent message and received message stored here
	string msg_out, msg_in;

	mpz_t sign_r, sign_s;
	mpz_t in_sign_r, in_sign_s;
	mpz_t sign_r_ver, sign_s_ver;

	// public key and signature of the hash of message E_pk(H(msg))
	mpz_t public_key;

	mpz_t hash_of_msg;

	// Constructor, destructor
	Client (int cli_id);
	//~Client();
	void gen_private();
	void gen_public();
	// saves message
	void save_msg(string msg);

	// sets hash_of_msg
	void hash_msg();

	// sets signature
	void sign();

	// sets msg_out
	void send_message(Client *cli);

	// takes in the signature and compares it with the stored msg_in
	bool verify_message(Client *cli);

	void set_msg_in(string msg);
private:
	mpz_t private_key;
};

#endif
