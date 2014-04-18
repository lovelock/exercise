#ifndef CHapticDeviceHandlerH
#define CHapticDeviceHandlerH

#include "deivces/CGenericHapticDevice.h"

#if defined(C_ENABLE_VIRTUAL_DEVICE_SUPPORT)
#include "devices/CVirtualDevice.h"
#endif

#if defined(C_ENABLE_DELTA_DEVICE_SUPPORT)
#include "devices/CDeltaDevices.h"
#endif

#if defined(C_ENABLE_FALCON_DEVICE_SUPPORT)
#include "devices/CFalconDevice.h"
#endif

#if defined(C_ENABLE_PHANTOM_DEVICE_SUPPORT)
#include "devices/CPhantomDevices.h"
#endif

const unsigned int C_MAX_HAPTIC_DEVICES = 16;

/*
	实现了一个列出所有不同设备的管理器，并提供了它们的句柄	
*/

class cHapticDeviceHandler
{
	public:
	cHapticDeviceHandler();
	virtual ~cHapticDeviceHandler();

	unsigned int getNumDevices() { return (m_numDevices); }
	void update();
	int getDeviceSpeficications(cHapticDeviceInfo& a_deviceSpecifications,
							unsigned int a_index = 0);
	int getDevice(cGenericHapticDevice*& a_hapticDevice,
							unsigned int a_index = 0);

	private:
	unsigned int m_numDevices;

	cGenericHapticDevice* m_devices[C_MAX_HAPTIC_DEIVCE];
	cGenericHapticDevice* m_nullHapticDevice;
};

#endif

