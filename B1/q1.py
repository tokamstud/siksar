cipher="ezmpzcyzeezmptedlbfpdetzy"
space=26
for i in range(1,26):
    decstr=""
    for j in range(0,len(cipher)):
        letter=(ord(cipher[j])-97)
        dec=((letter-i)%(space))+97
        decstr+=chr(dec)
    decrypt=decstr + " at key = " + repr(i)
    print (decrypt)

    
