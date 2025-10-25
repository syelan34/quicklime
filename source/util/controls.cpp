#include "controls.h"

#include "console.h"
#include "defines.h"
#include "ql_assert.h"
#include "ql_time.h"
#include "util/threads.h"
// #include "ir_user.h"
#include <malloc.h>
#include <3ds.h>
#include <citro3d.h>

#define M_RAD 0.01745329252f

namespace ql::controls {

	namespace // makes these inaccessible outside this namespace
	{
		LightLock _l	   = {1};
		unsigned int kDown = 0, kHeld = 0, kUp = 0, kRepeat = 0;
		angularRate gRate;
		C3D_FVec gPos = FVec3_New(0, 0, 0);
		circlePosition cPos, csPos;
		Thread controlsThread;
		bool cppConnected = false;
		volatile bool quitThread = false;
		//circlePadProInputResponse cppInputs, oldcppInputs;
		size_t refreshrate = 1/60.f * 1000000000;
		Handle connectionstatusevent, receivepacketevent;
		// gyro defaults
		float gyro_s = 0.5f, gyro_d = 10.f;
		unsigned int mappings[27] = {KEY_A,
									 KEY_B,
									 KEY_X,
									 KEY_Y,
									 KEY_SELECT,
									 KEY_START,
									 KEY_DRIGHT,
									 KEY_DLEFT,
									 KEY_DUP,
									 KEY_DDOWN,
									 KEY_L,
									 KEY_R,
									 KEY_ZL,
									 KEY_ZR,
									 KEY_CSTICK_UP,
									 KEY_CSTICK_DOWN,
									 KEY_CSTICK_LEFT,
									 KEY_CSTICK_RIGHT,
									 KEY_CPAD_UP,
									 KEY_CPAD_DOWN,
									 KEY_CPAD_LEFT,
									 KEY_CPAD_RIGHT,
									 KEY_DUP | KEY_CPAD_UP,
									 KEY_DOWN | KEY_CPAD_DOWN,
									 KEY_DLEFT | KEY_CPAD_LEFT,
									 KEY_DRIGHT | KEY_CPAD_RIGHT,
									 KEY_TOUCH};
		
		void ctrlupdate(void*) {
		    while (!quitThread) {
				hidScanInput();
    			hidGyroRead(&gRate);
    			hidCircleRead(&cPos);
    			hidCstickRead(&csPos);
                
    			kDown	= hidKeysDown();
    			kRepeat = hidKeysDownRepeat();
    			kHeld	= hidKeysHeld();
    			kUp		= hidKeysUp();
                
                //if (cppConnected) {
                //    Result res;
                //    res = svcWaitSynchronization(receivepacketevent, 0);
                //    if (R_SUCCEEDED(res)) {
                //        // iruserGetCirclePadProState(&cppInputs);
                //        csPos = cppInputs.cstick.csPos;
                //        
                //        if (oldcppInputs.status.r_pressed && !cppInputs.status.r_pressed) kDown |= KEY_R;
                //        else if (!oldcppInputs.status.r_pressed && !cppInputs.status.r_pressed) kHeld |= KEY_R;
                //        else if (!oldcppInputs.status.r_pressed && cppInputs.status.r_pressed) kUp |= KEY_R;
                //        if (oldcppInputs.status.zr_pressed && !cppInputs.status.zr_pressed) kDown |= KEY_ZR;
                //        else if (!oldcppInputs.status.zr_pressed && !cppInputs.status.zr_pressed) kHeld |= KEY_ZR;
                //        else if (!oldcppInputs.status.zr_pressed && cppInputs.status.zr_pressed) kUp |= KEY_ZR;
                //       if (oldcppInputs.status.zl_pressed && !cppInputs.status.zl_pressed) kDown |= KEY_ZL;
                //        else if (!oldcppInputs.status.zl_pressed && !cppInputs.status.zl_pressed) kHeld |= KEY_ZL;
                //        else if (!oldcppInputs.status.zl_pressed && cppInputs.status.zl_pressed) kUp |= KEY_ZL;
                //    }
                //}
                
                //oldcppInputs = cppInputs;
    
    			gRate.x *= gyro_s;
    			gRate.y *= gyro_s;
    			gRate.z *= gyro_s;
    
    			gPos.x += gRate.x * Time::deltaTime * M_RAD;
    			gPos.y += gRate.y * Time::deltaTime * M_RAD;
    			gPos.z += gRate.z * Time::deltaTime * M_RAD;
                svcSleepThread(refreshrate);
			}
			threadExit(0);
		}
		
		u32* sharedmem;
		u32 sharedmemsize = 0;
		
		static size_t round_up(size_t value, size_t multiple) {
            return (value / multiple + 1) * multiple;
        }
	}; // namespace

	void init() {
		LightLock_Init(&_l);
		Result res;
		res = hidInit();
		res = HIDUSER_EnableGyroscope();
		res = HIDUSER_EnableAccelerometer();
		
		sharedmemsize = round_up(0x30 + PACKET_BUFFER_SIZE + PACKET_BUFFER_SIZE, 0x1000);
        sharedmem = (u32*)memalign(sharedmemsize, 0x1000);
        ql::Console::log("shared mem size %p addr %p", sharedmemsize, sharedmem);
	    //res = iruserInit(sharedmem, sharedmemsize, PACKET_BUFFER_SIZE, PACKET_COUNT);
		Console::log("init %u", res);
		ASSERT(res == 0, "Failed to initialize IR:USER, result %p", (void*)R_DESCRIPTION(res));
        //res = IRUSER_GetConnectionStatusEvent(&connectionstatusevent);
        Console::log("connectionstatusevent %u", res);
        ASSERT(res == 0, "Failed to get connection status event");
        //res = IRUSER_GetReceiveEvent(&receivepacketevent);
        Console::log("receiveevent %u", res);
        ASSERT(res == 0, "Failed to get receive packet event");
		
		controlsThread = threadCreate(ctrlupdate, NULL, CONTROLS_THREAD_STACK_SZ, CONTROLS_THREAD_PRIORITY, CORE0, false);
	}
	
	void exit() {
        quitThread = true;
        threadJoin(controlsThread, U64_MAX);
        HIDUSER_DisableGyroscope();
        HIDUSER_DisableAccelerometer();
    }
    
    void setPollingRate(int rateHz) {
        ASSERT(rateHz > 0, "Invalid polling rate");
        refreshrate = 1.f/rateHz * 1000000000;
    }
    
    Result attemptConnectCirclePadPro() {
        //Result res = IRUSER_RequireConnection(1); // circle pad pro id is 1
    	//onsole::log("connection result %u", res);
        //if (R_FAILED(res)) return res;
        //res = iruserCPPRequestInputPolling(CPP_CONNECTION_POLLING_PERIOD_MS);
        //Console::log("input polling %u", res);
        //if (R_FAILED(res)) return res;
        //res = svcWaitSynchronization(receivepacketevent, 100000); // wait for 100ms for a response
        //onsole::log("waiting %u", res);
        //cppConnected = R_SUCCEEDED(res);
        return -1;
    }

	bool getDown(key inputName) {
		LightLock_Guard l(_l);
		return (kDown & mappings[(int)inputName]) == mappings[(int)inputName];
	}
	bool getRepeat(key inputName) {
		LightLock_Guard l(_l);
		return (kRepeat & mappings[(int)inputName]) == mappings[(int)inputName];
	}
	bool getHeld(key inputName) {
		LightLock_Guard l(_l);
		return (kHeld & mappings[(int)inputName]) == mappings[(int)inputName];
	}
	bool getUp(key inputName) {
		LightLock_Guard l(_l);
		return (kUp & mappings[(int)inputName]) == mappings[(int)inputName];
	}
	void setMapping(key keyName, unsigned int mapping) {
		LightLock_Guard l(_l);
		mappings[(unsigned char)keyName] = mapping;
	}
	const angularRate gyroRate() {
		LightLock_Guard l(_l);
		return gRate;
	}
	const C3D_FVec gyroPos() {
		LightLock_Guard l(_l);
		return gPos;
	};
	void resetGyro(const C3D_FVec newPos) {
		LightLock_Guard l(_l);
		gPos = newPos;
	};
	const circlePosition circlePos() {
		LightLock_Guard l(_l);
		return cPos;
	}
	const circlePosition cStickPos() {
		LightLock_Guard l(_l);
		return csPos;
	}
	float gyroSensitivity() {
		LightLock_Guard l(_l);
		return gyro_s;
	}
	void setGyroSensitivity(float sensitivity) {
		LightLock_Guard l(_l);
		gyro_s = sensitivity;
	}
	float gyroDeadZone() {
		LightLock_Guard l(_l);
		return gyro_d;
	}
	void setGyroDeadZone(float min_rate) {
		LightLock_Guard l(_l);
		gyro_d = min_rate;
	}
}; // namespace ql::controls