import socket
import re
import gzip
import struct

BAD_WORDS = ["spongebob", "Britney.Spears", "Paris.Hilton", "norrkoping", "norrk%C3%B6ping"]
BUFFER_SIZE = 1*1024*1024 # Defines a max buffer size as specified in requirement 5
RECV_SIZE = 1024

class Header:
    """
    HTTP Header class.
    Contains a dict with headers and the first line in the HTTP header.
    Header["host"] is an alias for Header.headers["host"]
    """
    def __init__(self, bytes):
        list = bytes.decode("utf-8").split("\r\n")
        self.first_line = list[0]
        self.headers = [item.split(":", 1) for item in list[1:-2]]
        self.headers = {key.lower():value.strip() for key, value in self.headers}

    def __setitem__(self, key, value):
        self.headers[key] = value

    def __getitem__(self, key):
        return self.headers[key]

    def get(self, key, default=None):
        return self.headers.get(key, default)

    def redirected_to(self, url):
        """ Makes header to HTTP 302 to with location = url """
        self.first_line = "HTTP/1.1 302 Found"
        self.headers = {}
        self.headers["location"] = "http://" + url
        self.headers["content-length"] = "0"

    def as_bytes(self):
        """ Converts header back to bytes for sending """
        bytes = self.first_line.encode("utf-8") + b"\r\n"
        for key, item in self.headers.items():
            bytes += key.encode("utf-8") + b": " + item.encode("utf-8") + b"\r\n"
        bytes += b"\r\n"
        return bytes

class HTTP:
    """ HTTP base class, inherited by request and response """
    def __init__(self):
        self.header = None
        self.content = b""
        self.content_length = 0
        self.content_read = 0
        self.content_to_large = False

    def recv_header(self, socket_):
        """
        Reads header and some of content
        Amount of content read = header_length % RECV_SIZE
        """
        header = b""
        content = b""
        while True:
            data = socket_.recv(RECV_SIZE)
            header += data
            if header == b"":
                return
            if b"\r\n\r\n" in header:
                header, content = header.split(b"\r\n\r\n")
                header += b"\r\n\r\n"
                break

        self.header = Header(header)

        self.content = content
        self.content_read = len(content)
        self.content_length = int(self.header.get("content-length", 0))

        if self.content_length > BUFFER_SIZE:
            self.content_to_large = True

    def recv_content(self, socket_):
        """ Receives content from socket, returns True if it read something
        otherwise it returns False. This makes looping easier as we can do:
        while response.recv_content():
            response.content
        """
        if len(self.content) >= BUFFER_SIZE:
            self.content = b""

        read_start = self.content_read

        while self.content_read < self.content_length:
            data = socket_.recv(RECV_SIZE)
            self.content_read += len(data)
            self.content += data
            if len(self.content) >= BUFFER_SIZE:
                break

        return self.content_read > read_start

    def send(self, socket_):
        """ sends header and content via socket_ """
        socket_.send(self.header.as_bytes() + self.content)


class Request(HTTP):
    """
    HTTP Request class.
    Used to store and parse HTTP Requests.
    """
    def __init__(self):
        super().__init__()

    def recv_header(self, socket_):
        """ Receives header and then handles it """
        super().recv_header(socket_)

        if self.header == None:
            return

        self.header.first_line = (self.header.first_line.replace("http://", "")).replace(self.header["host"], "")
        if ":" not in self.header["host"]:
            self.header["host"] += ":80"

        # If not HTTP connection we set header = None
        if not self.header["host"].endswith(":80"):
            self.header = None
            return

    def send(self, socket_):
        """ sends request to host via socket_ """
        host_addr, host_port = self.header["host"].split(":")
        host_ip = socket.gethostbyname(host_addr)
        host_port = int(host_port)

        socket_.connect((host_ip, host_port))
        super().send(socket_)

    def bad_url(self):
        """ Returns true if url contains bad words, false otherwise"""
        # Not a valid url but good enough to search for bad words
        str = self.header["host"] + self.header.first_line
        return contains_bad_words(str)


class Response(HTTP):
    """
    HTTP Response class.
    Used to store and parse HTTP Responses.
    """
    def __init__(self):
        super().__init__()

    def bad_content(self):
        """
        Will search text files not gzip encoded to find bad words
        in the content. Does this to be smart about what content
        to search for the forbidden keywords, as specified in requirement 8
        """
        if "text" not in self.header.get("content-type", ""):
            return False
        if "gzip" in self.header.get("content-encoding", ""):
            # Read last 4 bytes to determine the size of the uncompressed data
            size = struct.unpack('I', self.content[-4:])[0]
            if size > BUFFER_SIZE:
                return False
            data = gzip.decompress(self.content)
            return contains_bad_words(data.decode("utf-8"))
        return contains_bad_words(self.content.decode("utf-8"))

    def redirected_to(self, url):
        """ Changes header to a 302 Found redirect to url, and clears content """
        self.header.redirected_to(url)
        self.content = b""


def contains_bad_words(str):
    """ Uses regex to find bad words in str given, returns bool """
    for word in BAD_WORDS:
        if re.search(word, str, re.IGNORECASE):
            return True
    return False


def main():
    """ Main loop for accepting incoming requests """
    while True:
        request_socket, addr = server_socket.accept()

        request = Request()
        request.recv_header(request_socket)
        request.recv_content(request_socket)

        if request.header == None:
            request_socket.close()
            continue

        # If undesired URL redirect to error1.html as specified in requirement 3
        if request.bad_url():
            url = "zebroid.ida.liu.se/error1.html"
            response = Response()
            response.header = Header(b"HTTP/1.1 302 Found")
            response.redirected_to(url)
            response.send(request_socket)
            request_socket.close()
            continue

        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        request.send(client_socket)
        response = Response()
        response.recv_header(client_socket)

        # If response is larger then buffer size we send each full buffer at a
        # time as a response as specified in requirement 5.
        if response.content_to_large:
            request_socket.send(response.header.as_bytes())
            while response.recv_content(client_socket):
                request_socket.send(response.content)
            request_socket.close()
            client_socket.close()
            continue

        response.recv_content(client_socket)

        # If content contains inappropriate content bytes
        # redirect to error2.html as specified in requirement 4
        if response.bad_content():
            url = "zebroid.ida.liu.se/error2.html"
            response.redirected_to(url)

        response.send(request_socket)
        client_socket.close()
        request_socket.close()


if __name__ == "__main__":
    """ Set Port and start listening on server_socket  """
    IP = "127.0.0.1"
    PORT = 0

    # Allow the user to select port for the proxy, as specified in requirement 7
    while PORT < 1024:
        try:
            PORT = int(input("Enter a port number > 1024: "))
        except ValueError:
            pass

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)

    server_socket.bind((IP, PORT))
    server_socket.listen()
    print(" [*] Starting proxy server on " + IP + ":" + str(PORT))
    main()
