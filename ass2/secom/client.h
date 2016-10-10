#ifndef CLIENT_H
#define CLIENT_H

class Client {
public:
	mpz_t Pu_x;//public key of recipient
	mpz_t Pu_k;//public own key
	mpz_t Prime;
	mpz_t alpha;//primitive root
	Client(mpz_t Prime, mpz_t alpha);
	set_other_public(mpz_t Pu_x);
	void get_public(mpz_t ret);
private:
	mpz_t Pr_k;//Private key
	mpz_t Se_k; // Secret key for enc/dec
	void generate_private_key(mpz_t Prime, mpz_t alpha);
	void generate_secret_key(mpz_t Pu_x);
}

#endif
