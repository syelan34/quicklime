#include "meshrenderer.h"
#include "componentmanager.h"
#include "defines.h"
#include "mesh.h"
#include "ql_assert.h"
#include "shared_unif_locations.h"
#include "slmdlloader.h"
#include "stats.h"
#include "transform.h"
#include <citro3d.h>
// #include <concepts>
#include <string>

namespace ql {
	namespace {
		struct meshrenderer_args {
			unsigned int layer;
			char data;
			// mesh file
			// shader file
			// material file
		};
	} // namespace

	MeshRenderer::MeshRenderer(const void *data) { // parent will never be null
		ASSERT(data != nullptr, "Mesh parameter was null");
		meshrenderer_args &args = *(meshrenderer_args *)data;
		std::string meshpath	= &args.data;
		ASSERT(meshpath.size() > 0, "Model path is empty");
		Console::Log("Mesh path: %s", meshpath.c_str());
		std::string matpath = &args.data + meshpath.size() + 1;
		ASSERT(matpath.size() > 0, "Material path is empty");
		Console::Log("Material path: %s", matpath.c_str());
		mat = mdlLoader::parseMat(matpath);
		std::optional<std::shared_ptr<mesh>> mesh_opt =
			mdlLoader::parseModel(meshpath);
		ASSERT(mesh_opt.has_value(), "Invalid mesh");
		meshdata = mesh_opt.value();
		assert(mat != nullptr);
	}

	void MeshRenderer::render(C3D_Mtx &view, C3D_Mtx &projection) {
        // ASSERT(!parent.expired(), "Mesh renderer parent reference expired");

        // // set up buffers
        // C3D_SetBufInfo(&meshdata->buf);
        // C3D_SetAttrInfo(&meshdata->attrInfo);
		
		

		// always will have a transform
		// safe since pointer isn't stored
		// C3D_Mtx model = *parent.lock()->getComponent<Transform>();
		// C3D_Mtx inverse_model = model;
		// Mtx_Inverse(&inverse_model);
		// C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, ql::shared_unifs::matrix_m_loc, &model);
		// C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, ql::shared_unifs::matrix_im_loc, &inverse_model);
		
		// // set shader values
		// mat->setMaterial();

		// // LOD system
		// float distance2 = model.r[0].w * model.r[0].w +
		// 				  model.r[1].w * model.r[1].w +
		// 				  model.r[2].w * model.r[2].w;

		// for (LOD_info &inf : meshdata->LOD_levels) {
		// 	if (inf.distance2 <= distance2) {
		// 		C3D_DrawArrays(GPU_TRIANGLES, inf.beginindex, inf.size);
		// 		break;
		// 	}
		// }

		// mat->resetMaterial();
	}

	MeshRenderer &MeshRenderer::operator=(MeshRenderer &&other) {
		meshdata = std::move(other.meshdata);
		mat		 = std::move(other.mat);
		return *this;
	}

	MeshRenderer::MeshRenderer(MeshRenderer &&other) : meshdata(std::move(other.meshdata)), mat(std::move(other.mat)) {}

	std::shared_ptr<shader> MeshRenderer::material() const {
		return mat;
	}

	COMPONENT_REGISTER(MeshRenderer)
} // namespace ql