#include <iostream>
#include <math.h>
#include <gmp.h>

#include "../bigprime/bigprime.h"
#include "bigprime_test.h"

using namespace std;

void test_big_prime(char* val) {
	int psize = atoi(val);
	clock_t begin = clock();
	Bigprime bp (psize);
	mpz_out_str(stdout,10,bp.prime);cout<<endl;
	clock_t endtime = clock();
	cout << "Time "<< psize << "bit prime: " << float(endtime-begin)/CLOCKS_PER_SEC << " sek" <<endl;

	FILE* pfile;
	pfile = fopen("./primes/primes","w");
	mpz_out_str(pfile,10,bp.big_odd);
	fclose(pfile);
}

int main(int argc, char* argv[]) {

	test_big_prime(argv[1]);

	return 0;
}
