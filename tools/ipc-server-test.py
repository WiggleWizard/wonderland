import socket
import sys
import struct

# Create a UDS socket
sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

try:
    sock.connect('/tmp/wonderland')
except socket.error, msg:
    print >>sys.stderr, msg
    sys.exit(1)

payload = "RABBITHOLE"

packet = struct.pack('>I', 1)
packet += struct.pack('>I', len(payload))
packet += payload

sock.send(packet)
rx = sock.recv(1024)

print(list(bytearray(rx)))
print(rx)