import sys

def write(data):
    sys.stdout.buffer.write(data)
    sys.stdout.buffer.flush()

def read(size=1024):
    return sys.stdin.buffer.read1(size)

def main():
    rem_bytes = 0
    data = read()
    while data != b'':
        i = 0
        while i < len(data):
            if rem_bytes == 0:
                j = data.find(b'B', i)
                if j == -1:
                    data = data + read()
                    continue
                rem_bytes = int(data[i + len('Size: '):j].decode('ascii'))
                i = j + 1
            else:
                j = min(i + rem_bytes, len(data))
                rem_bytes -= j - i
                write(data[i:j])
                i = j
        data = read()

if __name__ == "__main__":
    main()
