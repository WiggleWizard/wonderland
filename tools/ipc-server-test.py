import socket
import sys
import struct

# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

try:
    sock.connect('/home/zinglish/unixtestsocket')
except socket.error, msg:
    print >>sys.stderr, msg
    sys.exit(1)

packet = "\xff\xff\xff\xfeTEST"

sock.send(packet)

print(list(bytearray(packet)))
print(packet)