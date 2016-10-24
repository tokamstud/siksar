#include <iostream>
#include <math.h>
#include <gmp.h>

#include <cryptopp/dh2.h>

#include "cli/client.h"
#include "hash/hash.h"

using namespace std;

int main(int argc, char* archv[]) {
	string message;
	unsigned int hash;
	unsigned long signature;
	bool trust;

	Client alice("alice");
	Client bobby("bobby");

	cout << "alice:";
	getline (cin, message);

	hash = alice.hash_msg(message);
	signature = alice.sign(hash);
	alice.send_message(message, signature, bobby);


	trust = bobby.verify_message();


	if (trust) {
		cout << "bobby believves that the message is from alice" << endl;
	} else {
		cout << "the verification step did not succese, bobby does not trust the sender" << endl;
	}
	return 0;
}
