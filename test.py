from ioctl import keepalive
import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
r = keepalive(s,(1, 1000, 1000))
print "succed  == 0 \n result == %s"%r



 