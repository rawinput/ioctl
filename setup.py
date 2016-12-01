from distutils.core import setup, Extension

module1 = Extension('ioctl',
                    sources = ['ioctl.c'],
					extra_link_args=["C:/MinGW/lib/libws2_32.a"])

setup (name = 'ioctl',
       version = '1.0',
       description = 'ioctl',
       ext_modules = [module1])