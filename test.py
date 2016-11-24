import ioctl
import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
r = ioctl.keepalive(s,(1, 10000, 3000)) 
print "succed  == 0 \n result == %s"%r
 