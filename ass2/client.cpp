#include<iostream>
#include<math.h>
#include<gmp.h>
#include <iomanip>


#include "cryptopp/modes.h"
#include "cryptopp/aes.h"
#include "cryptopp/filters.h"

// local
#include "client.h"
#include "bigprime/bigprime.h"

using namespace std;

Client::Client(mpz_t seed_prime, gmp_randstate_t state, string name) {
	this->client_name = name;
	mpz_init(this->Pu_x);
	mpz_init(this->Pu_k);
	mpz_init(this->Prime);
	mpz_init(this->alpha);
	mpz_init(this->Se_kn);

	make_alpha_Prime(seed_prime);
	generate_private_key(this->Prime, this->alpha, state);
	calc_public(this->Pu_k);

	// blum blum shut variables
	mpz_init(this->n);
}

Client::Client(mpz_t Prime, mpz_t alpha, gmp_randstate_t state, string name) {
	this->client_name = name;
	mpz_init(this->Pu_x);
	mpz_init(this->Pu_k);
	mpz_init(this->Prime);
	mpz_init(this->alpha);
	mpz_init(this->Se_kn);

	mpz_set(this->Prime,Prime);
	mpz_set(this->alpha,alpha);

	generate_private_key(this->Prime, this->alpha, state);
	calc_public(this->Pu_k);

	// blum blum shut variables
	mpz_init(this->n);

}

Client::~Client() {}

void Client::print_secret() {
	mpz_out_str(stdout,62,this->Se_k);cout<<endl;
}
void Client::print_nsecret() {
	mpz_out_str(stdout,62,this->Se_kn);cout<<endl;
}

void Client::generate_private_key(mpz_t Prime, mpz_t alpha, gmp_randstate_t state) {
	// TODO: set public key based on cyclic group, Prime is ok
	// random < Prime;

	mpz_t random;
	mpz_init(random);

	mpz_t i;
	mpz_init_set_str(i,"1",10);//i=1;

	mpz_t bound;
	mpz_init_set(bound, Prime);
	mpz_sub(bound,bound,i);//boud=Prime-1;

	mpz_urandomm(random, state, bound);
	mpz_init_set(this->Pr_k,random);
}
void Client::calc_public(mpz_t ret) {
	//takes initialized parameter to store the public number in, saves it
	mpz_powm(ret,this->alpha,this->Pr_k,this->Prime);// a^Pr_k mod p
	mpz_set(this->Pu_k,ret);// set field to ret
}

void Client::generate_secret_key() {
	// TODO: the secret key = Pu_x ^ Pr_k mod Prime
	mpz_init(this->Se_k);
	mpz_powm(this->Se_k,this->Pu_x,this->Pr_k,this->Prime);// Pu_x^Pr_k mod p
	mpz_set(this->Se_kn, this->Se_k);
	// run blum blum shub to generate key K, from K_ba
}

void Client::exchange_public(mpz_t Pu_x) {
	mpz_set(this->Pu_x, Pu_x);
}

// generating prime with primitive root/generator/alpha
void Client::make_alpha_Prime(mpz_t seed_prime) {
	mpz_t test;
	mpz_init(test);
	mpz_t c,s,i,prime;
	mpz_init(prime);
	mpz_init_set_str(c,"1",10);//arbitrary int c = 1;
	mpz_init_set_str(s,"2",10);//arbitrary int s = 2;
	mpz_init_set_str(i,"2",10);//arbitrary int i = 1;
	mpz_mul(prime,seed_prime,s);//seed=seed_prime*2;
	mpz_add(prime,prime,c);//prime+=1;
	do {//while a^q = 1 mod p
		mpz_add(i,i,c);//i++;
		mpz_powm(test,i,seed_prime,prime);
	} while(mpz_cmp(test,c)==0);

	mpz_set(this->alpha,s);
	mpz_set(this->Prime,prime);
}


void Client::find_bbs_init() {
	// Blum Blum Shub
	// generate two large prime numbers p x q = n
	// check if p=3mod4 and q=3mod4
	// check if this is false : p|key and q|key
	// if ok
	// key_next = key^2 mod n
	mpz_t mod,c,s,z;
	mpz_init_set_str(mod,"4",10);
	mpz_init_set_str(c,"3",10);
	mpz_init_set_str(s,"2",10);
	mpz_init_set_str(z,"0",10);
	int size = mpz_sizeinbase(this->Prime,2)/2;
	Bigprime p (size);
	Bigprime q (size);
	while (mpz_congruent_p(p.prime,c,mod)==0 && mpz_congruent_p(q.prime,c,mod)==0) {
		p.generate_prime();
		q.generate_prime();
	}
	while (mpz_congruent_p(this->Se_kn,z,p.prime)!=0 && mpz_congruent_p(this->Se_kn,z,q.prime)!=0) {
		p.generate_prime();
		q.generate_prime();
	}
	mpz_init(this->n);
	mpz_mul(this->n,p.prime,q.prime);
}

void Client::set_bbs_init(mpz_t n) {
	mpz_set(this->n,n);
}

void Client::get_bbs_init(mpz_t n) {
	mpz_set(n,this->n);
}

void Client::generate_next_key() {
	mpz_t s;
	mpz_init_set_str(s,"2",10);
	mpz_powm(this->Se_kn,this->Se_kn,s,this->n);
}

void dump_plain(string plaintext) {
	std::cout << "Plain Text (" << plaintext.size() << " bytes)" << std::endl;
    std::cout << plaintext;
    std::cout << std::endl << std::endl;
}

void dump_cipher(string ciphertext) {
	std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;

	for( int i = 0; i < ciphertext.size(); i++ ) {

		std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
	}

	std::cout << std::endl << std::endl;

}

string Client::encrypt(string plaintext) {
	byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
	memset( key, mpz_get_ui(this->Se_kn), CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

	string ciphertext;

	CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );

	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
	stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
	stfEncryptor.MessageEnd();

	cout << "---------------------------------------" << endl;
	cout << this->client_name << " is ENCRYPTING:" << endl;
	dump_plain(plaintext);
	dump_cipher(ciphertext);
	cout << "---------------------------------------" << endl;

	return ciphertext;
}

void Client::decrypt(string ciphertext) {
	byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
	memset( key, mpz_get_ui(this->Se_kn), CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

	string decryptedtext;

	CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );

	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
	stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
	stfDecryptor.MessageEnd();

	cout << "---------------------------------------" << endl;
	cout << this->client_name << " is DECRYPTING:" << endl;
	dump_cipher(ciphertext);
	dump_plain(decryptedtext);
	cout << "---------------------------------------" << endl;

}
