#ifndef CGenericDeviceH
#define CGenericDeviceH

#include "system/CGlobals.h"
#include "devices/CCallback.h"
#include <string.h>
#include <stdio.h>

using std::string;

class cGenericDevice
{
	public:
	cGenericDevice();
	virtual ~cGenericDevice() {};

	virtual int open() { return -1; }
	virtual int close() { return -1; }
	virtual int calibrate() { return -1; }
	virtual unsigned int getNumDevices() { return (0); }
	bool isDeviceAvailable() { return (m_deviceAvailable); }
	bool isDeviceReady() { return (m_deviceReady); }
	virtual bool setCallback(cCallback* a_callback);

	protected:
	bool m_deviceAvailable;
	bool m_deviceReady;

	cCallback* m_callback;
};

#endif
