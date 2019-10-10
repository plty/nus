#/usr/bin/env python
from socket import *
import sys

PORT = int(sys.argv[1])
def main():
    s = socket(AF_INET, SOCK_STREAM)
    s.connect(('localhost', PORT))
    s.settimeout(2)
    for line in sys.stdin:
        b = line.strip("\n").replace("%", "\n").encode('ascii')
        s.sendall(b)
        print("<<", b)
        print(">>", s.recv(1024))
    s.close()

if __name__ == "__main__":
    main()
