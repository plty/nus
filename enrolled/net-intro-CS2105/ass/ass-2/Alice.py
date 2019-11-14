import time
from functools import partial
from socket import *
from threading import Thread
from zlib import crc32
import sys

MAX_PACKET_SIZE = 64
ANY_HOST = ''


def to_int(b):
    return int.from_bytes(b, byteorder='big')

class Server:
    def __init__(self, port):
        self.start = 0
        self.port = port
        self.sock = socket(AF_INET, SOCK_DGRAM)

    @staticmethod
    def _chunk(msg, n):
        for i in range(0, len(msg), n):
            yield msg[i:i + n]

    @staticmethod
    def _chunk_str_stream(stream, n):
        for msg in stream:
            yield from Server._chunk(msg.encode('ascii'), n)

    @staticmethod
    def _pack(msg, sqn):
        sqn = sqn.to_bytes(4, byteorder='big')
        checksum = crc32(sqn + msg).to_bytes(4, byteorder='big')
        return checksum + sqn + msg

    def send(self, packets, window):
        while self.start != len(packets):
            for i in range(self.start, min(self.start + window, len(packets))):
                self.sock.sendto(packets[i], (ANY_HOST, self.port))

    @staticmethod
    def _unpack_ack(pack):
        checksum, sqn = pack[:4], pack[4:8]
        return to_int(checksum), to_int(sqn)

    def listen(self, n):
        while self.start != n:
            data, addr = self.sock.recvfrom(MAX_PACKET_SIZE)
            checksum, sqn = Server._unpack_ack(data)
            if checksum != crc32(data[4:]):
                continue
            self.start = max(self.start, sqn + 1)

def start_server(port):
    packets = [
                Server._pack(m, i) for i, m in 
                enumerate(Server._chunk(sys.stdin.buffer.read(), MAX_PACKET_SIZE - 8))
            ]
    server = Server(port)
    listen = Thread(target=partial(server.listen, len(packets))).start()
    send = Thread(target=partial(server.send, packets, 64)).start()


if __name__ == "__main__":
    start_server(int(sys.argv[1]))

