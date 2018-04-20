// dllmain.cpp : Defines the entry point for the DLL application.
#include "vrinputemulator.h"


#define PYEXPORT extern "C" __declspec(dllexport)
PYEXPORT
void test()
{
	printf("hello_world");
}

PYEXPORT
vrinputemulator::VRInputEmulator* create_connection()
{
	vrinputemulator::VRInputEmulator* emu = new vrinputemulator::VRInputEmulator();
	emu->connect();
	return emu;
}



PYEXPORT
uint32_t add_virtual_device(vrinputemulator::VRInputEmulator* emu, char* deviceSerial)
{
	uint32_t device;
	vrinputemulator::VirtualDeviceType device_type = vrinputemulator::VirtualDeviceType::TrackedController;
	device = emu->addVirtualDevice(device_type, deviceSerial, true);
	emu->publishVirtualDevice(device);
	return device;
}

PYEXPORT
void setVirtualDevicePose(vrinputemulator::VRInputEmulator *emu, uint32_t virtualDeviceId, const vr::DriverPose_t& pose, bool modal = true)
{

}


PYEXPORT
uint32_t get_virtual_device_count(vrinputemulator::VRInputEmulator* emu)
{
	return emu->getVirtualDeviceCount();
}

//vr::VRChaperoneSetup()->GetWorkingStandingZeroPoseToRawTrackingPose(&chaperoneMat); transformation between standing universe and driver space

//@My Es Zee just to clarify, you need to call vr::VRChaperoneSetup()->RevertWorkingCopy(); before the GetWorkingStandingZeroPose call
//and revertWorkingCopy isn't guaranteed to succeed
//I just call it every frame on my app

//https://github.com/sharkyh20/KinectToVR/blob/b104
