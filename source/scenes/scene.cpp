#include "scene.h"

#include "camera.h"
#include "console.h"
#include <citro3d.h>
#include "ql_time.h"
#include "shared_unif_locations.h"
#include <cmath>
#include "script.h"
#include <bullet/btBulletDynamicsCommon.h>
#include <threads.h>

namespace ql {
	Scene::Scene(std::string name): 
	    _name(name), 
		_broadphase(new btDbvtBroadphase()), 
		_collisionConfiguration(new btDefaultCollisionConfiguration()), 
		_dispatcher(new btCollisionDispatcher(_collisionConfiguration)), 
		_solver(new btSequentialImpulseConstraintSolver()), 
		_world(new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfiguration)), 
		name(_name) 
	{
		LightLock_Init(&lock);
	}

	Scene::~Scene() {
		Console::log("Scene destructor");
		scripts.clear();
		root.reset();
	}

	void Scene::awake() {
	    LightLock_Guard l(lock);
		act_on_scripts(&Script::Awake); // call awake() on every gameobject
											// and enable them (to self disable
											// do it when this is called)
	}
	
	void Scene::act_on_scripts(void (Script::*action)()) {
		for (auto &s : scripts)
			((*s).*action)();
	}

	void Scene::start() {
		LightLock_Init(&lock);
		LightLock_Guard l(lock);
		act_on_scripts(&Script::Start); // start all scripts
	}

	void Scene::update() {
		LightLock_Guard l(lock);
		act_on_scripts(&Script::Update); 
		// call update() on every gameobject

		// whatever other per frame logic stuff will get called here
		// reg.view<AudioSource>().each([](auto &as) { as.update(); });

		// call lateupdate() on every gameobject (propagates from root).
		// Used to ensure stuff like cameras move only when everything else is
		// done moving
		act_on_scripts(&Script::LateUpdate);
	};

	void Scene::fixedUpdate() {
		LightLock_Guard l(lock);
		act_on_scripts(&Script::FixedUpdate);
	};

	void Scene::draw() {
	    LightLock_Guard l(lock);
		
		C3D_FVUnifSet(GPU_VERTEX_SHADER, shared_unifs::time_loc, Time::curTime, std::sin(Time::curTime), std::cos(Time::curTime), Time::deltaTime);
		reg.view<Camera>().each([](auto &cam) { cam.Render(); });
	};
} // namespace ql