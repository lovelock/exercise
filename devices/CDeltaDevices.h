#ifndef CDeltaDeviceH
#define CDeltaDeviceH

#if defined(C_ENABLE_DELTA_DEVICE_SUPPORT)
#include "devices/CGenericHapticDevice.h"

#if defined(WIN32) | defined(WIN64)
#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define DHD_DEVICE_NONE			 0
#define DHD_DEVICE_3DOF			31
#define DHD_DEVICE_6DOF			61
#define DHD_DEVICE_6DOF_500		62
#define DHD_DEVICE_DELTA3		63
#define DHD_DEVICE_DELTA6		64
#define DHD_DEVICE_OMEGA		32
#define DHD_DEVICE_OMEGA33		34
#define DHD_DEVICE_OMEGA33_LEFT		36
#define DHD_DEVICE_OMEGA331		35
#define DHD_DEVICE_OMEGA331_LEFT	37
#define DHD_DEVICE_FALCON		60
#define DHD_DEVICE_CONTROLLER		81
#define DHD_DEVICE_CONTROLLER_HR	82
#define DHD_DEVICE_CUSTOM		91
#define DHD_DEVICE_DLR331	       102
#define DHD_DEVICE_DLR331_LEFT	       103
#define DHD_DEVICE_SIGMA331	       104
#define DHD_DEVICE_SIGMA331_LEFT       105


#define DHD_ON				 1
#define DHD_OFF				 0

#define DHD_MAX_DEVICE			 4
#define DHD_TIMEGAURD			 1
#define DHD_MAX_STATUS			14

#define DHD_STATUS_POWERED       0
#define DHD_STATUS_CONNECTED     1
#define DHD_STATUS_STARTED       2
#define DHD_STATUS_RESET         3
#define DHD_STATUS_IDLE          4
#define DHD_STATUS_FORCE         5
#define DHD_STATUS_BRAKE         6
#define DHD_STATUS_TORQUE        7
#define DHD_STATUS_WRIST_DETECTED 8
#define DHD_STATUS_ERROR         9
#define DHD_STATUS_GRAVITY       10
#define DHD_STATUS_TIMEGUARD     11
#define DHD_STATUS_ROTATE_RESET  12
#define DHD_STATUS_REDUNDANCY    13

#define DHD_MAX_BUTTONS          8

#define DHD_VELOCITY_WINDOWING   0
#define DHD_VELOCITY_AVERAGING   1

#endif
#endif


class cDeltaDevice : public cGenericHapticDevice
{
    public:
        cDeltaDevice(unsigned int a_deviceNumber = 0);

        virtual ~cDeltaDevice();

        int open();
        int close();
        int calibrate();
        unsigned int getNumDevices();
        int getPostion(cVector3d& a_position);
        int getLinearVelocity(cVector3d& a_linearVelocity);
        int getRotation(cMatrix3d& a_linearVelocity);
        int getRotation(cMatrix3d& a_rotation);
        int getGripperAngleRAD(double& a_angle);
        int setForce(const cVector3d& a_force);
        int setForceAndTorque(const cVector3d& a_force, const cVector3d& a_torque);
        int setForceAndTorqueAndGripperForce(const cVector3d& a_force, const cVector3d& a_torque, const cVector3d& a_gripperForce);
        int getUserSwitch(int a_switchIndex, bool& a_status);

        int getDeviceType() { return m_deviceType; }
        /* 覆盖设备上的力控制键 */
        int enableForces(bool a_value);
    protected:
        static int m_activeDeltaDevices;
        int m_deviceNumber;
        int m_deviceID;
        int m_deviceType;
        int m_userSwitchCount[8];
        int m_userSwitchStatus[8];
        bool statusEnableForceFirstTime;

        #ifndef DOXYGEN_SHOULD_SKIP_THIS
        static bool sdhdGetDeviceCount;
        static bool sdhdGetDeviceID;
        static bool sdhdGetSystemInfo;
        static bool sdhdOpenID;
        static bool sdhdClose;
        static bool sdhdReset;
        static bool sdhdGetButton;
        static bool sdhdGetPosition;
        static bool sdhdGetLinearVelocity;
        static bool sdhdGetOrientationRad;
        static bool sdhdSetTorque;
        static bool sdhdGetOrientationFrame;
        static bool sdhdSetForce;
        static bool sdhdSetForceAndTorque;
        static bool sdhdSetForceAndTorqueAndGripperForce;
        static bool sdhdGetGripperThumbPos;
        static bool sdhdGetGripperFingerPos;
        static bool sdhdGetGripperAngleRad;
        static bool sdhdEnableExperMode;
        static bool sdhdDisableExpertMode;
        static bool sdhdEnableForce;
        static bool sdhdIsLeftHanded;
        static bool sdhdSetBaseAngleZDeg;
        static bool sdhdSetVelocityThreshold;
        #endif
};
#endif
#endif
