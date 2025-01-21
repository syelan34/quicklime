#pragma once

#include "components/meshrenderer.h"
#include "util/dummyrenderer.h"
#include "util/renderqueue.h"
#include <variant>

namespace ql {
	class TextRenderer;
	class GameObject;

	enum RendererType {
		RENDERER_NONE,
		RENDERER_MESH,
		RENDERER_ANIM,
		RENDERER_TEXT,
		RENDERER_UI
	};

	// composition based renderer type
	class Renderer {
		std::variant<DummyRenderer, MeshRenderer> rnd;
		RendererType t;
		uint32_t _layer = 1; // by default on base layer (layer 1)
		GameObject *parent;

	  public:
		Renderer(GameObject &obj, const void *data);
		void render(C3D_Mtx &view, C3D_Mtx& projection);
		RenderQueue queue() const;
		uint32_t layer() const { return _layer; }
		Renderer& operator=(Renderer&&);
	};
} // namespace ql