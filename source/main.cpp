#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include "ql.h"
#include "systems/rendering.h"

namespace {
    void* stack_top = NULL;
    ERRF_ExceptionData exception_data = {};
    void handler(ERRF_ExceptionInfo *excep, CpuRegisters *regs) {
        ql::Console::Error("Main thread crashed");
        ql::Console::Error("Exception type: %d", excep->type);
        ql::Console::Error("PC: %p", regs->pc);
    }
	void prgrminit() {
	    threadOnException(handler, stack_top, &exception_data);
		romfsInit();
		osSetSpeedupEnable(true); // run way faster on n3ds
		ql::Console::init();
		ql::controls::init();
		ql::ComponentManager::init();
		ql::AudioManager::init();
		svcSetThreadPriority(CUR_THREAD_HANDLE, MAIN_THREAD_PRIORITY);
		// ql::physicsInit(21887825); // 20ms tick speed (approximately)
		// ql::physicsInit(54719563); // 50ms tick speed
		// ql::physicsInit(20.f);

		if (ql::SceneLoader::load(ql::Project::entrypointScene)) {
			ql::SceneManager::init();
		} else {
			// optionally exit or fallback
			ql::Project::programShouldExitGraceful = true;
		}
        ql::systems::Graphics::Init();
           	ql::Console::Error("Failed to load scene: %s", ql::Project::entrypointScene);
	}
	void update() {
		ql::SceneManager::update();
		ql::Console::update();
		ql::Time::Update();
	}

	void draw() {
		
	}
	void prgrmexit() {
		// ql::physicsExit();
		ql::controls::exit();
		ql::systems::Graphics::Exit();
		ndspExit();
		romfsExit();
	}
} // namespace



int main() {
	prgrminit();
	while (aptMainLoop() && !ql::Project::programShouldExitGraceful) {
		update();
		draw();
	}
	prgrmexit();
	return 0;
}
