#pragma once

#include <bullet/btBulletDynamicsCommon.h>

namespace ql {
    class GameObject;
	class RigidBody {
	    GameObject* parent;
		float mass;
		float drag;
		float angulardrag;
		bool automasscenter;
		bool usegravity;
		bool iskinematic;
		public:
		RigidBody(GameObject& parent, const void* data);
		RigidBody(RigidBody& other);
		RigidBody(RigidBody&& other);
		RigidBody& operator=(RigidBody& other);
		RigidBody& operator=(RigidBody&& other);
		void update();
	};
}