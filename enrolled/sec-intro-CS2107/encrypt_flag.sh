#!/bin/sh

# Generate a key
AES_KEY=`tr -cd '[:lower:]' < /dev/urandom | head -c 5`

# Print the plaintext
cat flag.txt | grep "flag{"

# Print the key
echo "Encryption key: $AES_KEY"

# Encrypt flag.txt using AES-128 in ECB mode and output ciphertext in Base64 to flag.txt.enc
openssl enc -aes-128-ecb -nosalt -base64 -pass pass:$AES_KEY -in flag.txt -out flag.txt.enc

# Remove plaintext flag
rm flag.txt
