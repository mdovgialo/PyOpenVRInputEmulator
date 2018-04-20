import ctypes
import os.path

ctypes.c_void_p
ctypes.c_wchar_p
dll = os.path.join(os.path.dirname(__file__), 'pyopenvr_emu_c_wrapper', 'x64', 'Release', 'pyopenvr_emu_c_wrapper.dll')

vrinputemu = ctypes.cdll.LoadLibrary(dll)
cr = vrinputemu.create_connection
cr.restype = ctypes.c_void_p
emu = vrinputemu.create_connection()
emu_p = ctypes.c_void_p(emu)
vrinputemu.get_virtual_device_count(emu_p)

add_dev = vrinputemu.add_virtual_device
add_dev.argtypes = [ctypes.c_void_p, ctypes.c_wchar_p]

add_dev(emu_p, ctypes.c_wchar_p("TEST@123"))

import IPython
IPython.embed()