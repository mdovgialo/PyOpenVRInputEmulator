// dllmain.cpp : Defines the entry point for the DLL application.
#include "vrinputemulator.h"
#include <iostream>

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


void setDeviceProperty(vrinputemulator::VRInputEmulator &ie, uint32_t deviceId, int dProp, std::string type, std::string value) {
	vr::ETrackedDeviceProperty deviceProperty = (vr::ETrackedDeviceProperty)dProp;
	if (std::strcmp(type.c_str(), "int32") == 0) {
		ie.setVirtualDeviceProperty(deviceId, deviceProperty, (int32_t)std::atoi(value.c_str()));
	}
	else if (std::strcmp(type.c_str(), "uint64") == 0) {
		ie.setVirtualDeviceProperty(deviceId, deviceProperty, (uint64_t)std::atoll(value.c_str()));
	}
	else if (std::strcmp(type.c_str(), "float") == 0) {
		ie.setVirtualDeviceProperty(deviceId, deviceProperty, (float)std::atof(value.c_str()));
	}
	else if (std::strcmp(type.c_str(), "bool") == 0) {
		ie.setVirtualDeviceProperty(deviceId, deviceProperty, std::atoi(value.c_str()) != 0);
	}
	else if (std::strcmp(type.c_str(), "string") == 0) {
		ie.setVirtualDeviceProperty(deviceId, deviceProperty, value.c_str());
	}
	else {
		throw std::runtime_error("Unknown value type.");
	}
}


PYEXPORT
uint32_t add_virtual_device(vrinputemulator::VRInputEmulator* emu, char* deviceSerial)
{

	try
	{
		// code that could cause exception
	
	uint32_t device;
	vrinputemulator::VirtualDeviceType device_type = vrinputemulator::VirtualDeviceType::TrackedController;
	device = emu->addVirtualDevice(device_type, deviceSerial, true);

	setDeviceProperty(*emu, device, 1000, "string", "lighthouse");
	setDeviceProperty(*emu, device, 1001, "string", "Vive Controller MV");
	setDeviceProperty(*emu, device, 1003, "string", "vr_controller_vive_1_5");
	setDeviceProperty(*emu, device, 1004, "bool", "0");
	setDeviceProperty(*emu, device, 1005, "string", "HTC");
	setDeviceProperty(*emu, device, 1006, "string", "1465809478 htcvrsoftware@firmware-win32 2016-06-13 FPGA 1.6/0/0 VRC 1465809477 Radio 1466630404");
	setDeviceProperty(*emu, device, 1007, "string", "product 129 rev 1.5.0 lot 2000/0/0 0");
	setDeviceProperty(*emu, device, 1010, "bool", "1");
	setDeviceProperty(*emu, device, 1017, "uint64", "2164327680");
	setDeviceProperty(*emu, device, 1018, "uint64", "1465809478");
	setDeviceProperty(*emu, device, 1029, "int32", "3");
	setDeviceProperty(*emu, device, 3001, "uint64", "12884901895");
	setDeviceProperty(*emu, device, 3002, "int32", "1");
	setDeviceProperty(*emu, device, 3003, "int32", "3");
	setDeviceProperty(*emu, device, 3004, "int32", "0");
	setDeviceProperty(*emu, device, 3005, "int32", "0");
	setDeviceProperty(*emu, device, 3006, "int32", "0");
	setDeviceProperty(*emu, device, 3007, "int32", "0");
	setDeviceProperty(*emu, device, 5000, "string", "icons");
	setDeviceProperty(*emu, device, 5001, "string", "{htc}controller_status_off.png");
	setDeviceProperty(*emu, device, 5002, "string", "{htc}controller_status_searching.gif");
	setDeviceProperty(*emu, device, 5003, "string", "{htc}controller_status_searching_alert.gif");
	setDeviceProperty(*emu, device, 5004, "string", "{htc}controller_status_ready.png");
	setDeviceProperty(*emu, device, 5005, "string", "{htc}controller_status_ready_alert.png");
	setDeviceProperty(*emu, device, 5006, "string", "{htc}controller_status_error.png");
	setDeviceProperty(*emu, device, 5007, "string", "{htc}controller_status_standby.png");
	setDeviceProperty(*emu, device, 5008, "string", "{htc}controller_status_ready_low.png");

	emu->publishVirtualDevice(device, false);

	auto pose = emu->getVirtualDevicePose(device);
	if (pose.deviceIsConnected != 1) {
		pose.deviceIsConnected = 1;
		pose.poseIsValid = 1;
		emu->setVirtualDevicePose(device, pose);
	}

	return device;
	}
	catch (const std::exception &exc)
	{
		// catch anything thrown within try block that derives from std::exception
		std::cout << exc.what();
	}
}


void connectDevice(vrinputemulator::VRInputEmulator *emu, uint32_t virtualDeviceId)
{

}

PYEXPORT
void set_virtual_device_pose(vrinputemulator::VRInputEmulator *emu, uint32_t virtualDeviceId, double x, double y, double z,
	double qx, double qy, double qz, double qw)
{
	auto pose = emu->getVirtualDevicePose(virtualDeviceId);
	pose.vecPosition[0] = x;
	pose.vecPosition[1] = y;
	pose.vecPosition[2] = z;
	pose.qRotation.w = qw;
	pose.qRotation.x = qx;
	pose.qRotation.y = qy;
	pose.qRotation.z = qz;
	emu->setVirtualDevicePose(virtualDeviceId, pose);
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
