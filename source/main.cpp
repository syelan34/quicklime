#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include "ql.h"
#include "systems/rendering.h"
#include "systems/physics.h"
#include "systems/scenes.h"

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
        ql::systems::Graphics::Init();
        ql::Console::init();
        ql::controls::init();
        ql::ComponentManager::init();
        ql::AudioManager::init();
        svcSetThreadPriority(CUR_THREAD_HANDLE, MAIN_THREAD_PRIORITY);
        // ql::systems::Physics::Init(20.f); // 20ms interval (50hz)
        
       	ql::systems::Scenes::Init();
        
        if (!ql::SceneLoader::load(ql::Project::entrypointScene)) {
           	ql::Console::Error("Failed to load scene: %s", ql::Project::entrypointScene);
           	// optionally exit or fallback
           	ql::Project::programShouldExitGraceful = true;
        }
	}
	void update() {
		ql::systems::Scenes::Update();
		ql::Console::update();
		ql::Time::Update();
	}

	void draw() {
		
	}
	void prgrmexit() {
		// ql::systems::Physics::Exit();
		ql::systems::Graphics::Exit();
		ql::controls::exit();
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
