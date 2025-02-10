#include "physics.h"
#include "console.h"
#include "defines.h"
#include "scenemanager.h"
#include "rigidbody.h"
#include "threads.h"
#include <3ds.h>

namespace {
	static Handle event, timer;
	static Thread physthreadhandle;
	static volatile bool exitphys = false;
	static volatile float timestep = 1.f / 50.f; // default of 50tps
	static TickCounter cnt = {};
	static void* stack_top = NULL;
	static ERRF_ExceptionData exception_data = {};
	
	static unsigned int idx = 0;
	float rollingavg(float val) {
	    static float ringbuf[256] = {0}; // last 256 values saved
		++idx;
		idx &= 0xFF;
		ringbuf[idx] = val;
		float sum = 0;
		for (int v : ringbuf) {
            sum += v;
        }
        return sum / 256.f;
	}
	void handler(ERRF_ExceptionInfo *excep, CpuRegisters *regs) {
	    ql::Console::error("Physics thread crashed");
		ql::Console::error("Exception type: %d", excep->type);
		ql::Console::error("PC: %p", regs->pc);
	}
} // namespace

void ql::physicsThread(void *) {
    threadOnException(handler, stack_top, &exception_data);
	Console::log("Physics thread start");
	// osTickCounterStart(&cnt);
	while (!exitphys) {
	    osTickCounterStart(&cnt);
		{
			LightLock_Guard l(SceneManager::lock);
			if (SceneManager::currentScene) {
			    SceneManager::currentScene->fixedUpdate();
			    SceneManager::currentScene->reg
				    .view<ql::RigidBody>()
					.each([](auto &rb) { rb.update(); });
				SceneManager::currentScene->_world->applyGravity();
				SceneManager::currentScene->_world->stepSimulation(timestep, 10);
				SceneManager::currentScene->_world->clearForces();
			}	
		}
		osTickCounterUpdate(&cnt);
		float timemillis = osTickCounterRead(&cnt);
		float avg = rollingavg(timemillis);
		Console::log("exp:%3.0f s:%2.3f e:%f a:%f", timestep, timemillis, timemillis - timestep, avg, avg);
		svcWaitSynchronization(timer, U64_MAX);
	}
	Console::log("Physics thread exit");
}

void ql::physicsInit(int tickspeed) {
    Console::log("int ver.");
    timestep = (tickspeed-2119489.f)/965914.f; // calculated experimentally
    Console::log("%f", timestep);
	svcCreateTimer(&timer, RESET_PULSE);
	svcSetTimer(timer, 0, tickspeed/10);
	svcCreateEvent(&event, RESET_ONESHOT);
	physthreadhandle = threadCreate(physicsThread, NULL, PHYSICS_THREAD_STACK_SZ, 0x18, -1, false);
}

void ql::physicsInit(float tickspeed) {
    Console::log("float ver.");
    timestep = tickspeed;
	svcCreateTimer(&timer, RESET_PULSE);
	svcSetTimer(timer, 0, (96591*tickspeed+211948)); // calculated experimentally
	svcCreateEvent(&event, RESET_ONESHOT);
	physthreadhandle = threadCreate(physicsThread, NULL, PHYSICS_THREAD_STACK_SZ, 0x18, -1, false);
}

void ql::physicsExit() { 
    exitphys = true;
    threadJoin(physthreadhandle, U64_MAX);
}