import sys
from zlib import crc32

def checksum(stream):
    return crc32(stream)

def main(argv):
    with open(argv[1], "rb") as f:
        stream = f.read()
        print(checksum(stream))

if __name__ == "__main__":
    main(sys.argv)

