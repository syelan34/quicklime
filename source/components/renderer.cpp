#include "renderer.h"
#include "dummyrenderer.h"
#include "meshrenderer.h"
#include "componentmanager.h"

namespace ql {
	namespace {
		struct renderer_args {
			ql::RendererType t;
			unsigned int layer;
		};
	} // namespace

	Renderer::Renderer(std::weak_ptr<GameObject> obj, const void *data) : parent(obj) {
		if (!data)
			return;
		const renderer_args& args = *static_cast<const renderer_args *>(data);
		switch (args.t) {
			case RENDERER_MESH:
				rnd.emplace<MeshRenderer>(obj, data);
				break;
			case RENDERER_TEXT: // not yet implemented
			case RENDERER_UI:
			default:
			break;
		}
	}

	void Renderer::render(C3D_Mtx &view, C3D_Mtx &projection) {
		std::visit([&](auto &renderer) { renderer.render(view, projection); }, rnd); // call render on whatever renderer is here
	}

	RenderQueue Renderer::queue() const {
		RenderQueue q;
		std::visit([&](auto &renderer) { q = renderer.material()->queue; }, rnd);
		return q;
	}

	Renderer& Renderer::operator=(Renderer&& other) {
		rnd = std::move(other.rnd);
		other.rnd.emplace<DummyRenderer>();
		_layer = other._layer;
		t = other.t;
		parent = other.parent;
		other.parent = {};
		return *this;
	}

	COMPONENT_REGISTER(Renderer)
}
