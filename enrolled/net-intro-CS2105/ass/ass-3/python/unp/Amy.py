from AESCipher import AESCipher
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Hash import MD5
from Crypto.PublicKey import RSA
from Crypto.Signature import PKCS1_PSS
from socket import *
import pickle
import secrets
import sys

HOST = ''
KEY_SIZE = 32
RECV_SIZE = 1024

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

def unpack_cred(cred):
    pub = pickle.loads(cred)
    sig = pickle.loads(cred[len(pickle.dumps(pub)):])
    return pub, sig

def verify_cred(pub, sig):
    m = MD5.new()
    m.update(b"bryan")
    m.update(pub)

    authority = read_key("berisign-python.pub")
    signer = PKCS1_PSS.new(authority)
    return signer.verify(m, sig)

def run_client(port):
    s = socket(AF_INET, SOCK_STREAM)
    s.connect((HOST, port))

    cred = s.recv(RECV_SIZE)
    while True:
        try:
            pub, sig = unpack_cred(cred)
            break
        except:
            cred += s.recv(RECV_SIZE)

    if not verify_cred(pub, sig):
        return
    
    key = RSA.importKey(pub)
    rsa = PKCS1_OAEP.new(key)

    sess_key = secrets.token_bytes(KEY_SIZE)
    enc_sess_key = rsa.encrypt(sess_key)

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

