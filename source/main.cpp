#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include "ql.h"

namespace {
    void* stack_top = NULL;
    ERRF_ExceptionData exception_data = {};
    void handler(ERRF_ExceptionInfo *excep, CpuRegisters *regs) {
        ql::Console::error("Main thread crashed");
        ql::Console::error("Exception type: %d", excep->type);
        ql::Console::error("PC: %p", regs->pc);
    }
	void prgrminit() {
	    threadOnException(handler, stack_top, &exception_data);
		gfxInitDefault();
		C3D_Init(C3D_DEFAULT_CMDBUF_SIZE * 64);
		C2D_Init(C2D_DEFAULT_MAX_OBJECTS);

#if CONSOLE
		consoleInit(GFX_BOTTOM, NULL);
#endif
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
			ql::Console::error("Failed to load scene: %s", ql::Project::entrypointScene);
			// optionally exit or fallback
			ql::Project::programShouldExitGraceful = true;
		}
	}
	void update() {
		ql::SceneManager::update();
		ql::Console::update();
		ql::Time::Update();
	}

	void draw() {
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		ql::SceneManager::draw();
		C3D_FrameEnd(GX_CMDLIST_FLUSH);
	}
	void prgrmexit() {
		// ql::physicsExit();
		ql::controls::exit();
		gfxExit();
		C3D_Fini();
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
