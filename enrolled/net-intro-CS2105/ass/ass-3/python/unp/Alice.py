from AESCipher import AESCipher
from Crypto.Cipher import PKCS1_OAEP
from Crypto.PublicKey import RSA
from socket import *
import pickle
import secrets
import sys

HOST = ''
KEY_SIZE = 32
RECV_SIZE = 4096

def read_file(filename):
    with open(filename) as f:
        return f.read()

def read_key(filename):
    key = read_file(filename)
    return RSA.importKey(key)

def recvall(s):
    data = b''
    recv = s.recv(RECV_SIZE)
    while recv:
        data += recv
        recv = s.recv(RECV_SIZE)
    return data

def run_client(port):
    key = read_key("bob-python.pub")
    rsa = PKCS1_OAEP.new(key)

    sess_key = secrets.token_bytes(KEY_SIZE)
    enc_sess_key = rsa.encrypt(sess_key)

    s = socket(AF_INET, SOCK_STREAM)
    s.connect((HOST, port))
    s.sendall(pickle.dumps(enc_sess_key))

    cipher = AESCipher(sess_key)
    messages = []
    data = recvall(s)
    while data:
        chunk = pickle.loads(data)
        data = data[len(pickle.dumps(chunk)):]
        messages.append(cipher.decrypt(chunk))

    with open('msgs.txt', 'wb') as f:
        f.write(b''.join(messages))

if __name__ == "__main__":
    _, port = sys.argv
    run_client(int(port))
