# Part I. Poly-alphabetic Ciphers

    $ pt1

file       | description
-|-
ciphertext  | file included in assignment, the ciphertext to decrypt
autokey.py  | python script that takes in *cipertext* file and decrypts it
ak_decryption.cpp | alternative version written in c++, unoptimized, wrote for testing purposes
ak_decryption     | binary file of * ak_decryption.cpp *

----------------------------------------------------------------------

    $ pt1/output

file | description
-----|------------
results.md      | textfile with the output from python program, generated on each run of *autokey.py*
plaintext.frag  | a textfile with the output from .cpp file that iis generated on each run of *ak_decryption*
----------------------------------------------------------------------
    $ pt1/ngrams

files | description
------|------------
bigrams, quadgrams, quintgrams, trigrams | lists of ngrams from a sample text

*
these were downloaded from
http://practicalcryptography.com/cryptanalysis/text-characterisation/quadgrams/
*

----------------------------------------------------------------------
### Running the programs
###### Python program

    $ python autokey.py

will take the *ciphertext* file and *ngrams/quadgram* file to compute the most
probable keyword that was used to encrypt the ciphertext

###### C++ program (very slow)

    $ g++ -o ak_decryption ak_decryption.cpp
    $ ./ak_decryption

this program does the same thing as the python program, has slightly different
output scores but manages to find the correct key anyway, it is unoptimized.


# Part II. Simplified DES
