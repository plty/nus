import sys

def chunk(s, n):
    return [s[i:i + n] for i in range(0, len(s), n)]

def to_ip(s):
    return map(lambda x: int(x, 2), chunk(s, 8))

def main(argv):
    print('.'.join(map(str, to_ip(argv[1]))))

if __name__ == "__main__":
    main(sys.argv)

