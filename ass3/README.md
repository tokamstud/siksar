# Author Tomasz Gliniecki

# Digital Signature with DSS and SHA512


folder contents:

bin: executable file

cli: client class and header file, used for this purpose

hash: hash function class and header file, sha512

./compile-run.sh: bash script that will compile source to the bin folder, and run the compiled program (only linux)

this file ontains also neccesary g+ flags in order to run the program.


Dependancies:

GMP library for arbitrary big number in c++


Installation:

run compile-run.sh
or

		$ g++ ../sign.cpp ../cli/client.cpp ../hash/hash.cpp -o sign -L/usr/lib/ -lgmp

form the root project directory (same as README, and compile-run.sh)

Running the program:

Program will first presend the public key components, and then prompt the user to type in a message to be sent to another client in this case we call them alice and bobby respectively. When Alice is done typing, hash of her message is displayed, next Bobby will compute verification step and confirm that the message originated from Alice.



sample output

public components:
	p:
38103326455490714404779469552282531460200708536740896025037841756193423681997852998086578745552976479826284040723902860171203363600227872693560987807435506198093272023185862643353746075891556476761172682569358844604843554697645190624984304277585653128661815874482969550350501524916614794810854065478972924987
	q:
523465928095167226519803150047553549822110568139
	g:
4128478855709576849280830897015870434029794080820115155844200352096574697650083489040052818855559985422994147041714592621305964970194768338155620999925565462268503248957453263909748696763140023381899630746988788435481802400034063047509177621717448773611279483649434262149218117496075030710393541816397120451
Time: 31.3632 sek
alice:grape

hash of the message that will be used for signing:
9375d1abdb644a01955bccad12e2f5c2bd8a3e226187e548d99c559a99461453b980123746753d07c169c22a5d9cc75cb158f0e8d8c0e713559775b5e1391fc4

Verifying that the checksum is correct :
29be6d8bb8069cfea70c7db749bc18ee961da033
29be6d8bb8069cfea70c7db749bc18ee961da033
bobby believves that the message is from alice
bobby received : grape
