#pragma once

#include "components/meshrenderer.h"
#include "util/dummyrenderer.h"
#include "util/renderqueue.h"
#include <variant>
#include <bullet/btBulletDynamicsCommon.h>

namespace ql {
	class TextRenderer;
	class GameObject;

	enum ColliderType {
		COLLIDER_NONE,
		COLLIDER_MESH, // concave mesh collider, REMEMBER MUST BE STATIC
		COLLIDER_PLANE,
		COLLIDER_CUBE,
		COLLIDER_SPHERE,
		COLLIDER_CYLINDER,
		COLLIDER_CAPSULE,
		COLLIDER_CONE,
		COLLIDER_CONVEX,
		COLLIDER_HEIGHTFIELD
	};

	// composition based renderer type
	class Collider {
		std::variant<DummyRenderer, MeshRenderer> rnd;
		RendererType t;
		u32 layer = 1; // by default on base layer (layer 1)
		GameObject *parent;
		btCollisionObject col;

	  public:
		Collider(GameObject &obj, const void *data);
		void render(C3D_Mtx &view, C3D_Mtx& projection, u32 mask);
		RenderQueue queue() const;
		Collider& operator=(Collider&&);
	};
} // namespace ql