# Author Tomasz Gliniecki

# Digital Signature with DSS and SHA-3


clinet1: "This is my message"
	$ calculating hash
	$ hash("This is my message")
client1: hash 9ie857tue
client1: send(E(PR_c1, hash) || message)

client2: receives message
	$ calculating hash of mesasge
	$ hash("This is my message")
client2: hash 9ie857tue
client2: D(PU_c1, hash')
client2: compare(D(PU_c1, hash') == hash)

if the same, message comes from client1 because only he can encrypt the correct hash to in order for PU_c1 to decrypt it.
