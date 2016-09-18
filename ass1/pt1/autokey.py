import itertools
import math
import collections
import timeit

def ngram_score(ngrams):
    length = 4
    n = sum(sample.values())
    for key in ngrams:
        num = (float(ngrams[key])/n)
        ngrams[key] = math.log10(num)
    return ngrams

def decrypt(ciphertex, key):
    decrypted = []
    for i in range(0,len(key)):
        c = ord(ciphertext[i])-65
        k = ord(key[(i%len(key))])-65
        p = chr( ( (c-k)%26 ) +65)
        decrypted.append(p)

    for i in range(0,len(ciphertext)-len(key)):
        c = ord(ciphertext[i+len(key)])-65
        k = ord(decrypted[i])-65
        p = chr(((c-k)%26)+65)
        decrypted.append(p)
    return decrypted

def splitToQuad(text):

    length = len(text)
    while length%4 != 0:
        length -= 1
        pass

    quads = []
    for k in range(4):
        quads += [text[i+k] + text[i+k+1] + text[i+k+2] + text[i+k+3] for i in range(0,length,4) if (i+k+3) < length]
        pass
    return quads;

def relevantChunks(text, offset,key):
#    klen = len(key)
#    for j in range(text):
#
#        for i in range(klen):
#            j+=1
#            if i > klen:
#                break
#            chars += text[i+j]
    raw = [text[i] + text[i+1] + text[i+2] for i in range(0,len(text),5)]
    relevantChunks = list(''.join(raw))
    return relevantChunks

def quadgramScore(text,key,offset,quadscore,totaln):
    # TODO : extract only relevant chunks based on key
    relevantText = relevantChunks(text, offset,key)
    myquads = collections.Counter(splitToQuad(relevantText))
    textProb = 0;
    for quad in myquads:
        if quad in quadscore:
            textProb += quadscore[quad]
            pass
        else:
            textProb += math.log10(0.1/totaln)
        pass

    return textProb;
def spaceout(plain):

    return

def loopThroughKeys(ciphertext,quadscore,totaln):
    result = {}
    keys = ["AA", "AAA", "AAAA", "AAAAA", "AAAAAA"]
    maxscore = -10000
    lastRoundMax = -10000
    saveScore = -10000;
    isTakeWholeKey = False
    i = 0;
    j = 0;
    k = 0;
    # for loop in python was annoying
    while(i<len(keys)):

        keyarr = list(keys[i])
        keyLength = len(keyarr)
        keyPass = 0;

        parentKey = list(keyarr)
        childKey = list(parentKey)

        j = 0;
        while(j<keyLength):
            # main logic
            k = 0;
            while(k<26):
                if isTakeWholeKey:
                    offset = keyLength
                else:
                    offset = j;

                childKey[j] = chr(65 + k)
                sendkey = ''.join(childKey)
                childDecryption = decrypt(ciphertext,sendkey)
                #
                # TODO: count out score
                #
                score = quadgramScore(childDecryption,childKey,offset,quadscore,totaln)

                if score > maxscore:
                    maxscore = score
                    parentKey = list(childKey)
                    saveScore = score;
                    pass

                k += 1
                pass
            childKey = []
            childKey = list(parentKey)
            # if last letter in current key:
            if j+1 == keyLength:
                isTakeWholeKey = True
                if (maxscore > lastRoundMax):
                    pass
                else:
                    break

                j=-1; # start from first letter current key again
                lastRoundMax = maxscore
                pass

            j += 1 # increment iteration
            pass
        lastRoundMax = -10000
        maxscore = -10000
        i += 1 # increment iteraction
        pass

        stri = ''.join(parentKey)
        keyprnt = "###### " + stri + " = " + str(saveScore)
        plain = ''.join(decrypt(ciphertext,stri))
        result[keyprnt] = plain + "\n"
        #print (keyprnt + '\n' + plain + '\n')

    return result


# getting ciphertext file
with open('./ciphertext','r') as cipherfile:
    ciphertext = cipherfile.read().replace(' ','').replace('\n','')

timeSample = timeit.default_timer()
# getting training sample
sample = {}
with open("./ngrams/quadgrams") as f:
    for line in f:
       (key,val) = line.split(' ')
       sample[key] = int(val)

# sum of quadgram occurances in the training sample
totaln = sum(sample.values())

# count logaritmig score of sample quadgrams
quadscore = ngram_score(sample)
stopSample = timeit.default_timer()

timeLoop = timeit.default_timer()
results = loopThroughKeys(ciphertext,quadscore,totaln)
stopLoop = timeit.default_timer()

con1 = "Sample calculations time | " + str(stopSample-timeSample)
con2 = "Evaluation of keys  time | " + str(stopLoop-timeLoop)
con3 = "Total  elapsed  time     | " + str(stopLoop-timeSample)
con4 = con1 + '\n' + con2 + '\n' + con3 + '\n'
# to console
for k in results:
    prnt = k + '\n' + results[k]
    print (prnt)
print (con4)

# to file
file = open('./output/results.md', 'w+')
file.write("## Python results \n desc.|time\n-|-\n")
file.write(con4)
for k in results:
    prnt = k + '\n' + results[k]
    file.write(prnt)
