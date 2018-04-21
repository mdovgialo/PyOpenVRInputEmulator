import ctypes
import os.path
from uuid import uuid4

import openvr
from math import cos, sin


def get_hmd_id():
    vrsys = openvr.VRSystem()
    for i in range(openvr.k_unMaxTrackedDeviceCount):
        device_class = vrsys.getTrackedDeviceClass(i)
        if device_class == openvr.TrackedDeviceClass_HMD:
            return i


class VRInputSystem:
    def __init__(self, global_offset, global_rotation):
        dll = os.path.join(os.path.dirname(__file__), 'pyopenvr_emu_c_wrapper', 'x64', 'Release',
                           'pyopenvr_emu_c_wrapper.dll')

        self.global_offset = global_offset
        self.global_rotation = global_rotation

        vrinputemu = ctypes.cdll.LoadLibrary(dll)
        self.vrinputemu = vrinputemu
        cr = vrinputemu.create_connection
        cr.restype = ctypes.c_void_p
        emu = vrinputemu.create_connection()
        emu_p = ctypes.c_void_p(emu)
        self.emu_p = emu_p

        openvr.init(openvr.VRApplication_Background)
        self.vrsys = openvr.VRSystem()

        hmd_id = get_hmd_id()
        self.hmd_id = hmd_id

        poses_t = openvr.TrackedDevicePose_t * openvr.k_unMaxTrackedDeviceCount
        self.poses = poses_t()

        set_pose = vrinputemu.set_virtual_device_pose
        set_pose.argtypes = [ctypes.c_void_p, ctypes.c_uint32, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                             ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,
                             ]
        self.set_tracker_pose = set_pose

    def add_tracker(self, name=None):
        add_dev = self.vrinputemu.add_virtual_device
        add_dev.argtypes = [ctypes.c_void_p, ctypes.c_wchar_p]
        add_dev.restype = ctypes.c_uint32

        if name is None:
            name = uuid4()
        dev = add_dev(self.emu_p, ctypes.c_wchar_p(name))
        tracker = VirtualTracker(dev, name, self)
        return tracker

    def tracker_count(self):
        return self.vrinputemu.get_virtual_device_count(self.emu_p)

    def get_hmd_quaternion(self):
        self.vrsys.getDeviceToAbsoluteTrackingPose(openvr.TrackingUniverseRawAndUncalibrated, 0, len(self.poses),
                                                   self.poses,)
        pose = self.poses[self.hmd_id]
        m = pose.mDeviceToAbsoluteTracking.m

        qw = (1 + m[0][0] + m[1][1] + m[2][2]) ** 0.5 / 2
        qx = (m[1][2] - m[2][1]) / (4 * qw)
        qy = (m[2][0] - m[0][2]) / (4 * qw)
        qz = (m[0][1] - m[1][0]) / (4 * qw)
        return qw, qx, -qy, qz

    def update_tracker(self, device_id, x, y, z, qw, qx, qy, qz):

        # rotate around z
        q = self.global_rotation[2]
        z = z
        x = x * cos(q) - y* sin(q)
        y = x* sin (q) + y*cos(q)

        # rotate around y
        q = self.global_rotation[1]
        z = z*cos(q) - x*sin(q)
        x = z*sin (q) + x*cos (q)

        # rotate around x
        q = self.global_rotation[0]
        y = y*cos (q) - z*sin (q)
        z = y*sin(q) + z*cos (q)

        x += self.global_offset[0]
        y += self.global_offset[1]
        z += self.global_offset[2]

        self.set_tracker_pose(self.emu_p, device_id, x, y, z, qx, qy, qz, qw)

class VirtualTracker():
    def __init__(self, dev, name, inputemu):
        self.x = 0
        self.y = 0
        self.z = 0

        self.device_id = dev
        self.name = name
        self.inputemu = inputemu

    def update(self, x ,y, z):
        self.x = x
        self.y = y
        self.z = z
        qw, qx, qy, qz = self.inputemu.get_hmd_quaternion()
        self.inputemu.update_tracker(self.device_id, x, y, z, qw, qx, qy, qz)