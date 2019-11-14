from socket import *
from random import getrandbits
from threading import Thread, Lock
from zlib import crc32
import sys

MAX_PACKET_SIZE = 64
ANY_HOST = ''

def inf_range(x=0):
    while True:
        yield x
        x += 1

def to_int(b):
    return int.from_bytes(b, byteorder='big')

class Client:
    def __init__(self, port):
        self.server = None
        self.port = port
        self.sock = socket(AF_INET, SOCK_DGRAM)
        self.messages = {}

        self.wait_on_server = Lock()
        self.wait_on_server.acquire()

    @staticmethod
    def _unpack(pack):
        checksum, sqn, msg = pack[:4], pack[4:8], pack[8:]
        return to_int(checksum), to_int(sqn), msg.decode('ascii')

    def listen(self):
        self.sock.bind((ANY_HOST, self.port))
        first = True
        while True:
            data, server = self.sock.recvfrom(MAX_PACKET_SIZE)
            if first:
                self.server = server
                self.wait_on_server.release()
                first = False
            checksum, sqn, msg = Client._unpack(data)
            if checksum != crc32(data[4:]):
                continue
            if sqn not in self.messages:
                self.messages[sqn] = msg

    @staticmethod
    def _pack_ack(sqn):
        sqn = sqn.to_bytes(4, byteorder='big')
        checksum = crc32(sqn).to_bytes(4, byteorder='big')
        return checksum + sqn


    def write(self):
        self.wait_on_server.acquire()
        for sqn in inf_range():
            while True:
                if sqn in self.messages:
                    self.sock.sendto(Client._pack_ack(sqn), self.server)
                    print(self.messages[sqn], end='')
                    break

                # one in 256 loop it will resend ack.
                if sqn != 0 and getrandbits(8) == 0:
                    self.sock.sendto(Client._pack_ack(sqn - 1), self.server)



def start_client(port):
    client = Client(port)
    listen = Thread(target=client.listen).start()
    send = Thread(target=client.write).start()


if __name__ == "__main__":
    start_client(int(sys.argv[1]))

