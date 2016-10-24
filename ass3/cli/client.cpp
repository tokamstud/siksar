#include "client.h"
#include "../hash/hash.h"

Client::Client(string name) {
	this->name = name;
}
Client::~Client() {}


void Client::hash_msg(string msg, mpz_t hash) {
	// TODO: use hash/hash.h header file include
	// calculate hash and set its value to mpz_t hash
	return 0;
}
