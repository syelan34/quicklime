#include "systems/physics.h"
#include "systems/scenes.h"
#include "console.h"
#include "defines.h"
#include "threads.h"
#include <3ds.h>

static btBroadphaseInterface*                  _broadphase;
static btDefaultCollisionConfiguration*        _collisionConfiguration;
static btCollisionDispatcher*                  _dispatcher;
static btSequentialImpulseConstraintSolver*    _solver;
static btDiscreteDynamicsWorld*                _world;

static void physicsThread(void *);
static Handle event, timer;
static Thread physthreadhandle;
static volatile bool exitphys = false;
static volatile float timestep = 1.f / 50.f; // default of 50tps
static TickCounter cnt = {};
static void* stack_top = NULL;
static ERRF_ExceptionData exception_data = {};

void ql::systems::Physics::Init(float interval) {
    _broadphase = new btDbvtBroadphase();
	_collisionConfiguration = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);
	_solver = new btSequentialImpulseConstraintSolver();
	_world = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration);
	
	Console::Log("float ver.");
    svcCreateTimer(&timer, RESET_PULSE);
    svcSetTimer(timer, 0, (96591*interval+211948)); // calculated experimentally on O3DS
    svcCreateEvent(&event, RESET_ONESHOT);
    physthreadhandle = threadCreate(physicsThread, NULL, PHYSICS_THREAD_STACK_SZ, 0x18, -1, false);
}

void ql::systems::Physics::Exit() {
    exitphys = true;
    threadJoin(physthreadhandle, U64_MAX);
    
    // Deallocate Physics World
    delete _world;
    delete _solver;
    delete _collisionConfiguration;
    delete _broadphase;
    delete _dispatcher;
}

static unsigned int idx = 0;
static float rollingavg(float val) {
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
static void handler(ERRF_ExceptionInfo *excep, CpuRegisters *regs) {
    ql::Console::Error("Physics thread crashed");
	ql::Console::Error("Exception type: %d", excep->type);
	ql::Console::Error("PC: %p", regs->pc);
}

static void physicsThread(void *) {
    threadOnException(handler, stack_top, &exception_data);
	ql::Console::Log("Physics thread start");
	
	osTickCounterStart(&cnt);
	while (!exitphys) {
	    osTickCounterStart(&cnt);
		{
			// if (SceneManager::currentScene) {
			//     SceneManager::currentScene->fixedUpdate();
			//     SceneManager::currentScene->reg
			// 	    .view<ql::RigidBody>()
			// 		.each([](auto &rb) { rb.update(); });
				
			// }
			_world->applyGravity();
			_world->stepSimulation(timestep, 10);
			_world->clearForces();
		}
		osTickCounterUpdate(&cnt);
		float timemillis = osTickCounterRead(&cnt);
		float avg = rollingavg(timemillis);
		ql::Console::Log("exp:%3.0f act:%2.3f err:%f avg:%f", timestep, timemillis, timemillis - timestep, avg, avg);
		svcWaitSynchronization(timer, U64_MAX);
	}
	ql::Console::Log("Physics thread exit");
}