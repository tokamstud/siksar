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
Py-results.md      | textfile with the output from python program, generated on each run of *autokey.py*
C++-results.md  | C++ output textfile with the output that is generated on each run of
\_*.md files    | old output files saves

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
----------------------------------------------------------------------
----------------------------------------------------------------------


# Part II. Simplified DES

    $ pt2

files | description
------|------------
sdes.cpp | C++ file that creates output for Task1-3
sdes     | compiled binary file
cxt1.txt | 480 bit long SDES encrypted text
cxt2.txt | 480 bit long T2SDES encrypted text

    $ pt2/output

files | description
------|------------
plaintext.md   | the output generated from running the C++ program
\_plaintext.md | copy of the file

### Running the program
###### C++

    $ g++ -o sdes sdes.cpp
    $ ./sdes


sdes will create output file ./output/plaintext.md
