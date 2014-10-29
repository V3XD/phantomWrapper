#include <windows.h>
#include <stdio.h>
#include <assert.h>
#include <conio.h>
#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HL/hl.h>
#include <HDU/hduVector.h>
#include <iostream>
     
#define DLLEXPORT __declspec(dllexport)
     
extern "C"
{
	DLLEXPORT bool initDevice(void);
	DLLEXPORT void cleanup(void);
	DLLEXPORT bool getData(void);
	DLLEXPORT double getPosX(void);
	DLLEXPORT double getPosY(void);
	DLLEXPORT double getPosZ(void);
	DLLEXPORT double getQuatX(void);
	DLLEXPORT double getQuatY(void);
	DLLEXPORT double getQuatZ(void);
	DLLEXPORT double getQuatW(void);
	DLLEXPORT bool isButtonADown(void);
	DLLEXPORT bool isButtonBDown(void);
	DLLEXPORT double gimbalX(void);
	DLLEXPORT double gimbalY(void);
	DLLEXPORT double gimbalZ(void);
	DLLEXPORT double jointX(void);
	DLLEXPORT double jointY(void);
	DLLEXPORT double jointZ(void);
}
     
/* Handle to haptic device. */
HHD ghHD = HD_INVALID_HANDLE;

/* Handle to haptic rendering context. */
HHLRC ghHLRC;

HLdouble position[3];
HLdouble orientation[4];
HLboolean buttonA = 0;
HLboolean buttonB = 0;
hduVector3Dd gimbalAngles;
hduVector3Dd jointAngles;
double transMatrix[16];
     
DLLEXPORT bool initDevice(void)
{
	HDErrorInfo error;

	ghHD = hdInitDevice(HD_DEFAULT_DEVICE);
	if (HD_DEVICE_ERROR(error = hdGetError()))
	{
		return false;
	}
	
	// Create a haptic context for the device.  The haptic context maintains 
	// the state that persists between frame intervals and is used for
	// haptic rendering.
	ghHLRC = hlCreateContext(ghHD);
	hlMakeCurrent(ghHLRC);
	
	hlDisable(HL_USE_GL_MODELVIEW);
	
	return true;
}

DLLEXPORT void cleanup(void)
{
	hlMakeCurrent(NULL);
	hlDeleteContext(ghHLRC);
	hdDisableDevice(ghHD);
}

DLLEXPORT bool getData(void)
{
	bool errorBool = true;
	HDErrorInfo error;

	// Start haptic frame.
	hlBeginFrame();
	// End the frame.
	hlEndFrame();  

	hlGetDoublev(HL_DEVICE_POSITION, position);
		
	hlGetDoublev(HL_DEVICE_ROTATION, orientation);

	hlGetBooleanv(HL_BUTTON1_STATE, &buttonA);

	hlGetBooleanv(HL_BUTTON2_STATE, &buttonB);

	hdGetDoublev(HD_CURRENT_GIMBAL_ANGLES, gimbalAngles);

	hdGetDoublev(HD_CURRENT_JOINT_ANGLES, jointAngles);

	//hdGetDoublev(HD_CURRENT_TRANSFORM, transMatrix);

	if(HD_DEVICE_ERROR(error = hdGetError()))
		errorBool = false;

	return errorBool;
}

DLLEXPORT double getPosX(void)
{
	return position[0];
}

DLLEXPORT double getPosY(void)
{
	return position[1];
}

DLLEXPORT double getPosZ(void)
{
	return position[2];
}

DLLEXPORT double getQuatX(void)
{
	return orientation[0];
}

DLLEXPORT double getQuatY(void)
{
	return orientation[1];
}

DLLEXPORT double getQuatZ(void)
{
	return orientation[2];
}

DLLEXPORT double getQuatW(void)
{
	return orientation[3];
}

DLLEXPORT bool isButtonADown(void)
{
	bool result = false;
	if(buttonA)
		result = true;
	return result;

}

DLLEXPORT bool isButtonBDown(void)
{
	bool result = false;
	if(buttonB)
		result = true;
	return result;
}

DLLEXPORT double gimbalX(void)
{
	return gimbalAngles[0];
}

DLLEXPORT double gimbalY(void)
{
	return gimbalAngles[1];
}

DLLEXPORT double gimbalZ(void)
{
	return gimbalAngles[2];
}

DLLEXPORT double jointX(void)
{
	return jointAngles[0];
}

DLLEXPORT double jointY(void)
{
	return jointAngles[1];
}

DLLEXPORT double jointZ(void)
{
	return jointAngles[2];
}