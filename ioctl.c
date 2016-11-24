#include <Python.h>
#include <winsock2.h>

#ifndef _MSTCPIP_
#define _MSTCPIP_

#ifdef __LP64__
#pragma push_macro("u_long")
#undef u_long
#define u_long __ms_u_long
#endif

struct tcp_keepalive {
	u_long onoff;
	u_long keepalivetime;
	u_long keepaliveinterval;
};

#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)
#define SIO_RCVALL_MCAST _WSAIOW(IOC_VENDOR,2)
#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR,4)
#endif /* _MSTCPIP_ */

#ifdef MS_WINDOWS
typedef SOCKET SOCKET_T;
#       ifdef MS_WIN64
#               define SIZEOF_SOCKET_T 8
#       else
#               define SIZEOF_SOCKET_T 4
#       endif
#else
typedef int SOCKET_T;
#       define SIZEOF_SOCKET_T SIZEOF_INT
#endif
typedef int64_t _PyTime_t;
typedef struct {
	PyObject_HEAD
	SOCKET_T sock_fd;           /* Socket file descriptor */
	int sock_family;            /* Address family, e.g., AF_INET */
	int sock_type;              /* Socket type, e.g., SOCK_STREAM */
	int sock_proto;             /* Protocol type, usually 0 */
	PyObject *(*errorhandler)(void); /* Error handler; checks
									 errno, returns NULL and
									 sets a Python exception */
	_PyTime_t sock_timeout;     /* Operation timeout in seconds;
								0.0 means non-blocking */
} PySocketSockObject;


static PyObject *
set_error(void)
{
#ifdef MS_WINDOWS
    int err_no = WSAGetLastError();
    /* PyErr_SetExcFromWindowsErr() invokes FormatMessage() which
       recognizes the error codes used by both GetLastError() and
       WSAGetLastError */
    if (err_no)
        return PyErr_SetExcFromWindowsErr(PyExc_OSError, err_no);
#endif

    return PyErr_SetFromErrno(PyExc_OSError);
}



static PyObject*
sock_ioctl(PySocketSockObject *s , PyObject *arg)
{
	PyObject *argO;
	DWORD recv;
	struct tcp_keepalive ka;
	if (!PyArg_ParseTuple(arg, "O(kkk):keepalive",&argO,
		&ka.onoff, &ka.keepalivetime, &ka.keepaliveinterval))
		return NULL;

	if (WSAIoctl(PyObject_AsFileDescriptor(argO), SIO_KEEPALIVE_VALS, &ka, sizeof(ka),
		NULL, 0, &recv, NULL, NULL) == SOCKET_ERROR) {
		return set_error();
	}
	return PyLong_FromUnsignedLong(recv);
}


static PyMethodDef IoctlMethods[] = {
	{ "keepalive",  sock_ioctl, METH_VARARGS,
	  "Windows WSAIoctl implementation." },
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC
initioctl(void)
{
	(void)Py_InitModule("ioctl", IoctlMethods);
}