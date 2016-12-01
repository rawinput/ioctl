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
sock_ioctl(PyObject *argO , PyObject *arg)
{
	PyObject *s;
	DWORD recv;
	struct tcp_keepalive ka;
	if (!PyArg_ParseTuple(arg, "O(kkk):keepalive",&s,
		&ka.onoff, &ka.keepalivetime, &ka.keepaliveinterval))
		return NULL;

	if (WSAIoctl(PyObject_AsFileDescriptor(s), SIO_KEEPALIVE_VALS, &ka, sizeof(ka),
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