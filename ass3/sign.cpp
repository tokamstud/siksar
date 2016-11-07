#include <iostream>
#include <math.h>
#include <gmp.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <thread>

#include <cryptopp/dh2.h>

#include "cli/client.h"
#include "hash/hash.h"

using namespace std;

void generate_public_comp(Client *cli1, Client *cli2) {
	mpz_t inve, num,ajn, modul; mpz_init(inve);
/*
	mpz_init_set_str(modul,"11",10);
	mpz_init_set_str(ajn,"1",10);
	mpz_init_set_str(num,"3",10);

	mpz_invert (inve, num, modul);

	mpz_out_str(stdout,10,inve);cout<<endl;
*/
	// find strong prime
	mpz_t one, two, test; mpz_init(test);
	mpz_init_set_str (one, "1", 10);
	mpz_init_set_str (two, "2", 10);

	gmp_randstate_t state;gmp_randinit_mt(state);
	unsigned long int seed = time(NULL);
	gmp_randseed_ui(state, seed);

	unsigned long int power = 1024;

	mpz_t p;mpz_init(p);
	mpz_t q;mpz_init(q);
	mpz_t s;mpz_init(s);

	bool confirm = false;

	clock_t beginSDES = clock();

	int a = 0;
	int b = 0;
	int i = 0;
	while (!confirm) {
		mpz_init_set_str (q, "0", 10);
		mpz_init_set_str (s, "0", 10);

		while (mpz_probab_prime_p(q, 25) == 0) {
			mpz_urandomb (q, state, 160);
		}
		//mpz_out_str(stdout,10,q);cout<< endl << endl;
		while (mpz_probab_prime_p(s, 25) == 0) {
			mpz_urandomb (s, state, 864);
		}
		//mpz_out_str(stdout,10,s);cout<< endl << endl;

		// p = 2sq + 1 :
		mpz_mul (p, q, s);
		mpz_mul (p, p, two);
		mpz_add (p, p, one);

		a = mpz_probab_prime_p (p, 25); // if prime then a > 0
		if (a > 0) {
			confirm = true;
		}
	}

	mpz_t g; mpz_init(g);
	mpz_t h; mpz_init(h);
	mpz_t p_1; mpz_init(p_1);
	mpz_sub (p_1, p_1, one);

	mpz_t p_q; mpz_init(p_q);
	mpz_mul(p_q, s, two);

	bool t_flag = false;
	while (!t_flag) {
		mpz_powm (g, two, p_q, p);
		cout << "test" << endl;
		if (mpz_cmp_ui(g,1) > 0) {
			t_flag = true;
		} else {
			mpz_add_ui(two, two, 1);
		}
	}

	// p, q, g are acccesible here
	mpz_set(cli1->p, p);
	mpz_set(cli1->q, q);
	mpz_set(cli1->g, g);

	mpz_set(cli2->p, p);
	mpz_set(cli2->q, q);
	mpz_set(cli2->g, g);


	clock_t endSDES = clock();
	cout << "public components: " << endl;
	cout << "	p: " << endl;
	mpz_out_str(stdout,10,p);cout<<endl;
	cout << "	q: " << endl;
	mpz_out_str(stdout,10,q);cout<<endl;
	cout << "	g: " << endl;
	mpz_out_str(stdout,10,g);cout<<endl;

	cout << "Time: " << float(endSDES-beginSDES)/CLOCKS_PER_SEC << " sek" << endl;


}

int main(int argc, char* archv[]) {
	string message;
	mpz_t hash;
	bool trust;

	Client alice(00001);
	Client bobby(00002);
	// will assign public values to clients
	generate_public_comp(&alice, &bobby);
	alice.gen_private();
	alice.gen_public();
	bobby.gen_private();
	bobby.gen_public();

	// creating message
	cout << "alice:";
	getline (cin, message);
	alice.save_msg(message);

	// creating hash of message
	alice.hash_msg();

	// signing the hash of message
	alice.sign();

	// sending message and signature
	alice.send_message(&bobby);

	trust = bobby.verify_message(&alice);

	if (trust) {
		cout << "bobby believves that the message is from alice" << endl;
		cout << "bobby received : " << bobby.msg_in << endl;
	} else {
		cout << "the verification step did not succeed, bobby does not trust the sender" << endl;
	}

/*
	string input = "grape";
	string output1 = sha512(input);

	cout << "sha512('"<< input << "'):" << output1 << endl;

*/
	return 0;
}
