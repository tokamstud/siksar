#include <cstring>
#include <fstream>
#include<gmp.h>

#include "hash.h"


void sh512_h::init(){
	m_h[0] = 0x6a09e667f3bcc908ULL;
	m_h[1] = 0xbb67ae8584caa73bULL;
	m_h[2] = 0x3c6ef372fe94f82bULL;
	m_h[3] = 0xa54ff53a5f1d36f1ULL;
	m_h[4] = 0x510e527fade682d1ULL;
	m_h[5] = 0x9b05688c2b3e6c1fULL;
	m_h[6] = 0x1f83d9abfb41bd6bULL;
	m_h[7] = 0x5be0cd19137e2179ULL;
	m_len = 0;
	m_tot_len = 0;
}
// generating prime with primitive root/generator/alpha
void sha510_test_big(mpz_t seed_prime) {
	mpz_t test;
	mpz_init(test);
	mpz_t c,s,i,prime;
	mpz_init(prime);
	mpz_init_set_str(c,"1",10);//arbitrary int c = 1;
	mpz_init_set_str(s,"2",10);//arbitrary int s = 2;
	mpz_init_set_str(i,"2",10);//arbitrary int i = 1;

	// 2*q + 1 = p
	// if p is not prime get new q
	mpz_mul(prime,seed_prime,s);//prime=seed_prime*2;
	mpz_add(prime,prime,c);//prime+=1;

	do {
		mpz_add(i,i,c);//i++;
		mpz_powm(test,i,seed_prime,prime);
	} while(mpz_cmp(test,c)==0);

}

void sh512_h::update(const unsigned char *messege, unsigned int len){
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifter_message;
	tmp_len = SHA384_512_BLOCK_SIZE - m_len;
	rem_len = len < tmp_len ? len : tmp_len;
	memcpy(&m_block[m_len], messege, rem_len);
	if (m_len + len < SHA384_512_BLOCK_SIZE) {
		m_len += len;
		return;
	}
	new_len = len - rem_len;
	block_nb = new_len / SHA384_512_BLOCK_SIZE;
	shifter_message = messege + rem_len;
	transform(m_block, 1);
	transform(shifter_message, block_nb);
	rem_len = new_len % SHA384_512_BLOCK_SIZE;
	memcpy(m_block, &shifter_message[block_nb << 7], rem_len);
	m_len = rem_len;
	m_tot_len += (block_nb + 1) << 7;
}
void sh512_h::transform(const unsigned char *messege, unsigned int block_nb)
{
	uint64 w[80];
	uint64 wv[8];
	uint64 t1, t2;
	const unsigned char *sub_block;
	int i, j;
	for (i = 0; i < (int) block_nb; i++) {
		sub_block = messege + (i << 7);
		for (j = 0; j < 16; j++) {
			SHA2_PACK64(&sub_block[j << 3], &w[j]);
		}
		for (j = 16; j < 80; j++) {
			w[j] =  SHA512_F4(w[j -  2]) + w[j -  7] + SHA512_F3(w[j - 15]) + w[j - 16];
		}
		for (j = 0; j < 8; j++) {
			wv[j] = m_h[j];
		}
		for (j = 0; j < 80; j++) {
			t1 = wv[7] + SHA512_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6])
				+ sha512_k[j] + w[j];
			t2 = SHA512_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}
		for (j = 0; j < 8; j++) {
			m_h[j] += wv[j];
		}

	}
}

void sh512_h::final(unsigned char *digest){
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;
	int i;
	block_nb = 1 + ((SHA384_512_BLOCK_SIZE - 17)
					 < (m_len % SHA384_512_BLOCK_SIZE));
	len_b = (m_tot_len + m_len) << 3;
	pm_len = block_nb << 7;
	memset(m_block + m_len, 0, pm_len - m_len);
	m_block[m_len] = 0x80;
	SHA2_UNPACK32(len_b, m_block + pm_len - 4);
	transform(m_block, block_nb);
	for (i = 0 ; i < 8; i++) {
		SHA2_UNPACK64(m_h[i], &digest[i << 3]);
	}
}
bool test_if(mpz_t doublecheck) {
	mpz_t c;mpz_init(c);mpz_set_str(c,"1",10);
	mpz_t doublecheck_1;mpz_init(doublecheck_1);mpz_set(doublecheck_1,doublecheck);
	mpz_sub(doublecheck_1,doublecheck_1,c);

	mpz_t random;mpz_init(random);
	mpz_t test;mpz_init(test);

	for (size_t i = 0; i < 25; i++) {
		mpz_powm(test,random,doublecheck_1,doublecheck);
		if (mpz_cmp(test,c)!=0) {
			return false;
		}
	}
	return true;
}

std::string sha512(std::string input) {

	unsigned char digest[sh512_h::DIGEST_SIZE];
	memset(digest,0,sh512_h::DIGEST_SIZE);
	sh512_h ctx = sh512_h();
	ctx.init();
	ctx.update((unsigned char*)input.c_str(), input.length());
	ctx.final(digest);

	char buf[2*sh512_h::DIGEST_SIZE+1];
	buf[2*sh512_h::DIGEST_SIZE] = 0;
	for (int i = 0; i < sh512_h::DIGEST_SIZE; i++)
		sprintf(buf+i*2, "%02x", digest[i]);
	return std::string(buf);
}

  /*
  clock_t timeThread = clock();
  vector<int> T_keys;
  std::thread t1 (T_brutx_T2SHA512,cxt2,std::ref(T_keys),0,256,"t1");
  std::thread t2 (T_brutx_T2SHA512,cxt2,std::ref(T_keys),256,512,"t2");
  std::thread t3 (T_brutx_T2SHA512,cxt2,std::ref(T_keys),512,768,"t3");
  std::thread t4 (T_brutx_T2SHA512,cxt2,std::ref(T_keys),1002,1024,"t4");
  t1.join();
  t2.join();
  t3.join();
  t4.join();

  std::unique_lock<std::mutex> lck(mtx);
  while (!ready) cv.wait(lck);
  cout << endl << endl;
  cout << T_keys[0] << endl;
  cout << T_keys[1] << endl;
  clock_t stopThread = clock();

  cout << "Time TIME: " << float(stopThread-timeThread)/CLOCKS_PER_SEC << endl;
  */
  const unsigned long long sh512_h::sha512_k[80] =
  			{0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
  			 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
  			 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
  			 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
  			 0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
  			 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
  			 0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
  			 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
  			 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
  			 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
  			 0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
  			 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
  			 0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
  			 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
  			 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
  			 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
  			 0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
  			 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
  			 0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
  			 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
  			 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
  			 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
  			 0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
  			 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
  			 0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
  			 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
  			 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
  			 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
  			 0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
  			 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
  			 0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
  			 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
  			 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
  			 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
  			 0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
  			 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
  			 0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
  			 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
  			 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
  			 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL};
/*
  str_to_file(out_table_SHA512.append(out_table_T2SHA512).append(outSHA512).append(outT2SHA512).append(outTOTAL),output); // write result to output file

*/
