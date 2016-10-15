### Assignment 1
## Implement Secure Communications

	./bigprime:
	|	|- bigprime.cpp // class file for bigprime ./implemented:
	|	|- bigprime.h // header file to be linked with secom.cpp
	|
	./tests:
	|	|- test.sh // can be run on linux to run tests
	|
	|- client.cpp  // class file
	|- client.h   // header file to be lined with secom.cpp
	|- secom.cpp  // main file to compile
	|
	./primes // some primes in textform base62

# Main folder

		source file for the main program
	secom.cpp

		client class and header file, class used by alice and bob
	client.h
	client.cpp


# bigprime folder

		class and header used to produce arbitrary big prime numbers, this is used in the main program, and needs to be linked under compilation
	bigprime.h
	bigprime.pp


# test folder

		this is folder for testing and generating primes, not required for the main program to run

		test.sh can be run on linux to compile and run tests
	test.sh

## Requirements (for linux)

* GMP GNU Library (gmp) (for arbitrary big numbers)
https://gmplib.org/#DOWNLOAD

* Crypto++ (cryptopp) library for AES implementation
http://www.cryptopp.com/wiki/Linux

both of these libraries are found in most popular package managers like pacman, or apt-get
e.g.:

	$ pacman -S cryptop++
	$ pacman -S gmp

or

	$ apt-get install libgmp3-dev
	$ apt-get install libcryptop++8 libcrypto++8-dbg libcryptop++-dev

## Compilation and running (installation)
	$ g++ -lgmp -lcryptopp secom.cpp client.cpp bigprime/bigprime.cpp -o secom

	$ ./secom

OR

	$ ./run-secure.sh

* to close the program send C^ (default)

## Description

The program will first produce 2048bit prime number, this is the most time consming part ( < 5sek).
Then the output with public information for DH Key exchange.

Next the prompt for client1 is displayed, plaintext + ciphertext, then whetever client2 recieved ciphertext and decrypted text.

# Sample output

The cyclic group and public parameters used
Cyclic group 2048bit:
GsNspbSevU1UHvwP7qlAdzo69VdQ7jXDn5yKCZyeux0iKw7oyaHKr7ptfUXt8ydWYCiM6tucnXBDSr7emap71lDhqXFpZRV8Yh7wvVkVdU8wKlOoyUpkq9aMbxBGpaTYfkyT5vFyjG4pIpcFJbD9ZcWh7bu4w3PiSv1eR75sDh62QbLHjnh0iCJAc6Q92JNANlDC60Zk0WzujS48ZvBxWmFNq3UpOjKLjLIvJbhNV1azpgkBLCD9ZmykbUPdZta8ZHsmEjkWPcPFgShg4SDqK0YTr0Msjra7dAbHxLC8Svr7dut06K8m7QGAAn2CX4yNUeHMOFpJ2nBjBkyiskwP87ox
The generator:
2

The shared seed key K_ab:
	Alice:
cIb9SOP5WxZEFIENAybqZfi9BMBVx0YqIVVfcdgaqhRfR1XuSSNDpHXzl00YAVq8V4k7IXlwSoqAqFtN7F2jKGudR0ZizDgMM3aqSd7dsN0zEariZUoW01hHoICU0DiQ6bRnqTxIARAguNwFjgYhjsENn7Jl4VmgytHA3rtNWJlO7yqHulmhQLRTalwR72vnWy9I7JB9aPUIzxBmql5zvTz5cjAZ9nh1sADFvijpH6T6VhkStA2rzqo8EtPLBaL0vneaq9vhrMvYfiXxN71emhEZrr1C30kBGNHaU2tJ84Ay7DHhpFUZXXHcP8wNOgWrFwatB4xdgio65sSI4yKrWPd
	Bobby:
cIb9SOP5WxZEFIENAybqZfi9BMBVx0YqIVVfcdgaqhRfR1XuSSNDpHXzl00YAVq8V4k7IXlwSoqAqFtN7F2jKGudR0ZizDgMM3aqSd7dsN0zEariZUoW01hHoICU0DiQ6bRnqTxIARAguNwFjgYhjsENn7Jl4VmgytHA3rtNWJlO7yqHulmhQLRTalwR72vnWy9I7JB9aPUIzxBmql5zvTz5cjAZ9nh1sADFvijpH6T6VhkStA2rzqo8EtPLBaL0vneaq9vhrMvYfiXxN71emhEZrr1C30kBGNHaU2tJ84Ay7DHhpFUZXXHcP8wNOgWrFwatB4xdgio65sSI4yKrWPd

The shared key K for encryption:
	Alice:
ITS0TtWW9eywGcSiiyooDgRYDhmWTLfef9PXevWWPqiHre65kXecuB55hT3S6a5s4bnbYyoKM0n4zcaKlx9ojzGSGggy42fDrAeoJXZUa05x5Ae1KvjGTIBRYCLHpNLc6Q44AfBjaqcgyhwA1kTWuksZzMJNr3p00fVb8jTv1u6xBy3AC0QXB2LV02BugNz3Q7uWj0SYB91vCdWQZUr6wDLXQACvLS7R8vo8T4OHWWK4qWL2NvhnzoT1Rl8Z4BTBGSvsbDd3H2Xj1CjFOroCSSFtDygaZvFcE3FdOgeN3ZEWAYsIAywasy50nuxMFIxXf5RSKeRdUMMJnqZf1rL4Vo6
	Bobby:
ITS0TtWW9eywGcSiiyooDgRYDhmWTLfef9PXevWWPqiHre65kXecuB55hT3S6a5s4bnbYyoKM0n4zcaKlx9ojzGSGggy42fDrAeoJXZUa05x5Ae1KvjGTIBRYCLHpNLc6Q44AfBjaqcgyhwA1kTWuksZzMJNr3p00fVb8jTv1u6xBy3AC0QXB2LV02BugNz3Q7uWj0SYB91vCdWQZUr6wDLXQACvLS7R8vo8T4OHWWK4qWL2NvhnzoT1Rl8Z4BTBGSvsbDd3H2Xj1CjFOroCSSFtDygaZvFcE3FdOgeN3ZEWAYsIAywasy50nuxMFIxXf5RSKeRdUMMJnqZf1rL4Vo6

alice:Hi thi is alice
---------------------------------------
Alice is ENCRYPTING:
Plain Text (15 bytes)
Hi thi is alice

Cipher Text (32 bytes)
0x7c 0xd1 0xcd 0x3f 0xff 0x76 0x70 0x18 0x91 0x93 0x44 0x41 0xc5 0x77 0x34 0xe5 0xb0 0xe5 0x41 0xbe 0xe1 0xf6 0x92 0x4c 0xe 0x68 0x61 0xa 0x92 0x32 0x51 0x3a

---------------------------------------
---------------------------------------
Bobby is DECRYPTING:
Cipher Text (20 bytes)
0x7c 0xd1 0xcd 0x3f 0xff 0x76 0x70 0x18 0x91 0x93 0x44 0x41 0xc5 0x77 0x34 0xe5 0xb0 0xe5 0x41 0xbe 0xe1 0xf6 0x92 0x4c 0xe 0x68 0x61 0xa 0x92 0x32 0x51 0x3a

Plain Text (10 bytes)
Hi thi is alice

---------------------------------------
bobby:Hi Alice, this is Bobby!
---------------------------------------
Bobby is ENCRYPTING:
Plain Text (18 bytes)
Hi Alice, this is Bobby!

Cipher Text (20 bytes)
0x7b 0x6a 0xda 0x5 0x45 0xa9 0x32 0xc3 0xb5 0x53 0xc2 0xdb 0x44 0x7e 0xfe 0x2d 0x8c 0x52 0x77 0xbf 0x67 0x1b 0x9c 0x7e 0xb4 0xf1 0xaf 0x3c 0x21 0xfc 0x1f 0x8a

---------------------------------------
---------------------------------------
Alice is DECRYPTING:
Cipher Text (20 bytes)
0x7b 0x6a 0xda 0x5 0x45 0xa9 0x32 0xc3 0xb5 0x53 0xc2 0xdb 0x44 0x7e 0xfe 0x2d 0x8c 0x52 0x77 0xbf 0x67 0x1b 0x9c 0x7e 0xb4 0xf1 0xaf 0x3c 0x21 0xfc 0x1f 0x8a

Plain Text (19 bytes)
Hi Alice, this is Bobby!

---------------------------------------
