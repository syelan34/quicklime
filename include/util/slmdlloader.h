#pragma once

#include <memory>
#include <optional>
#include <string>

namespace ql {
	class shader;
	class bone;
	class mesh;
	class GameObject;
	namespace mdlLoader {
		struct mdl_header {
			char mdl[4]			  = "";
			unsigned int numVerts = 0, sv = 0;
			float radius		  = 0.f;
			unsigned int attrnum;
			unsigned char attrtypes[16], attrlen[16];
		};

		/**
		 * @brief Creates a shader instance from a material file
		 *
		 * @param f The material file to read from
		 * @return material* A pointer to the created material
		 */
		[[nodiscard]] std::unique_ptr<shader> parseMat(const std::string file);

		/**
		 * @brief Creates a skeleton structure from model file
		 *
		 * @param f The model file pointer
		 * @return bone* The skeleton
		 */
		[[nodiscard]] bone *parseBones(FILE *f);

		[[nodiscard]] std::optional<std::shared_ptr<mesh>>
		parseModel(const std::string &path, bool createnew = false);
	} // namespace mdlLoader

} // namespace ql
