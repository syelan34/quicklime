#pragma once

#include "shaders/color.h"
#include "shaders/texture.h"
#include "util/renderqueue.h"
#include <citro3d.h>
#include <memory>
#include <optional>
#include <stdio.h>
#include <string>

namespace ql {
#define float3torgb32(color)                  \
	((((int)(color[0] * 255) & 0xFF) << 24) | \
	 (((int)(color[1] * 255) & 0xFF) << 16) | \
	 (((int)(color[2] * 255) & 0xFF) << 8) | (0xFF << 0))

	class shader {
	  protected:
		/**
		 * @brief Reads null-terminated string from file
		 *
		 * @param [in] fid File pointer to read from
		 * @param [out] str Output char buffer to write to
		 * @param [in] max_size Length of string to not exceed
		 * @return int Size of the returned string
		 */
		static int freadstr(FILE *fid, char *str, size_t max_size);
		/**
		 * @brief
		 *
		 * @param [out] tex
		 * @param [out] cube
		 * @param data Texture data
		 * @param size Size of texture data
		 * @return true if texture exists
		 * @return false if texture does not exist
		 */
		static bool loadTextureFromMem(C3D_Tex *tex, C3D_TexCube *cube,
									   const void *data, size_t size);
		/**
		 * @brief Load a texture from romfs by name.
		 *
		 * @param name The texture name
		 * @return A shared pointer to the texture. If the name is blank, will
		 * load a blank white texture. If the specified name cannot be found,
		 * the return value will be empty.
		 */
		static std::optional<std::shared_ptr<Texture>> loadTextureFromFile(std::string name);

	  public:
		RenderQueue queue;
		virtual ~shader()			 = 0;
		virtual void resetMaterial() = 0;
		virtual void setMaterial()	 = 0;

		void setFVUniform(std::string name, C3D_FVec value);
		void setFVUniform(int id, C3D_FVec value);
		void setBUniform(std::string name, bool value);
		void setBUniform(int id, bool value);
		void setIVUniform(std::string name, C3D_FVec value);
		void setIVUniform(int id, C3D_FVec value);
	};
} // namespace ql
