# ioctl 

the ioctl.pyd is your module put it into your project

usage :

	import ioctl
	import socket
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	r = ioctl.keepalive(s,(1, 10000, 3000))

