#ifndef CLIENT_H
#define CLIENT_H
#include <stdint.h>
#include <gmp.h>

using namespace std;

class Client {
public:
	string name;
	string msg_out, msg_in;

	mpz_t public_key;
	mpz_t signature;
	void hash_msg(string msg, mpz_t hash);
	void sign_hash(mpz_t hash, mpz_t signature);
	void send_message(string msg, mpz_t signature, Client cli);
	bool verify_message(string msg, mpz_t signature)
private:
	mpz_t private_key;
};

#endif
