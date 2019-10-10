#/usr/bin/env python
from socket import *
import sys

ANY_HOST = ''
HEADER_SEPARATOR = b'  '
RECV_SIZE = 32
KV_STORE_PREFIX = b'/key/'

class Request:
    def __init__(self, method, path, header, body):
        self.method = method
        self.path = path
        self.header = header
        self.body = body

    @staticmethod
    def _chunk(l, n):
        for i in range(0, len(l), n):
            yield l[i:i + n]

    @staticmethod
    def from_raw_header(header):
        header = header.split(b' ')
        method, path = header[0].upper(), header[1]
        header = map(lambda x: (x[0].lower(), x[1]), Request._chunk(header[2:], 2))
        return Request(method, path, dict(header), b'')

class Response:
    desc = {200: "OK", 404: "NotFound", 405: "MethodNotAllowed"}

    def __init__(self, body=None, status=200):
        self.body = body
        self.status = status

    def to_bytes(self):
        if self.body is not None:
           return "{} {} Content-Length {}  ".format(
                self.status,
                self.desc[self.status],
                len(self.body)
                ).encode("ascii") + self.body
        else:
            return "{} {}  ".format(
                self.status, 
                self.desc[self.status]
                ).encode("ascii")


class Server:
    def __init__(self, port):
        self.port = port
        self.kv_store = Redisnt()

    def start(self):
        sock = socket(AF_INET, SOCK_STREAM)
        sock.bind((ANY_HOST, self.port))
        sock.listen(1)
        while True:
            conn, addr = sock.accept()
            self._handle_conn(conn, addr)

    def _handle_conn(self, conn, addr):
        rem_bytes = 0
        req = None

        data = conn.recv(RECV_SIZE)
        body = []
        while data:
            while data:
                if rem_bytes == 0:
                    x = data.find(HEADER_SEPARATOR)
                    if x == -1:
                        break

                    raw_header = data[:x]
                    req = Request.from_raw_header(raw_header)
                    rem_bytes = int(req.header.get(b'content-length', b'0'))
                    data = data[x + len(HEADER_SEPARATOR):]
                else:
                    x = min(rem_bytes, len(data))
                    rem_bytes -= x
                    body.append(data[:x])
                    data = data[x:]

                if req and rem_bytes == 0:
                    req.body = b''.join(body)
                    body = []
                    print(req.method, req.path, req.header, len(req.body))
                    res = self._handle_req(req)
                    req = None
                    print(">>", res.status)
                    conn.send(res.to_bytes())

            data += conn.recv(RECV_SIZE)
        conn.close()


    def _handle_req(self, req):
        if req.path.startswith(KV_STORE_PREFIX):
            return self.kv_store.on_request(req)
        else:
            return Response(None, 404)

# Isnt Redis
class Redisnt:
    def __init__(self):
        self.store = {}

    def put(self, key, value):
        self.store[key] = value
        return Response()

    def get(self, key):
        if key in self.store:
            return Response(self.store[key])
        return Response(None, 404)

    def delete(self, key):
        if key in self.store:
            return Response(self.store.pop(key))
        return Response(None, 404)

    def on_request(self, req):
        key = req.path[len(KV_STORE_PREFIX):]
        method = req.method
        if method == b"GET":
            return self.get(key)
        elif method == b"POST":
            return self.put(key, req.body)
        elif method == b"DELETE":
            return self.delete(key)
        return Response(None, 405)

def start_server(port):
    server = Server(port)
    server.start()


if __name__ == "__main__":
    start_server(int(sys.argv[1]))

