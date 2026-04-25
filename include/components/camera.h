#pragma once

#include <3ds.h>
#include <citro3d.h>
#include <memory>
#include <utility>
#include <vector>

namespace ql {
	class GameObject;
	class Renderer;
	class Transform;

	using iod_func = float (*)(float);

	class Camera {
		friend class GameObject;
	  private:
		static bool cameraObjectListDirty;
		gfxScreen_t display;
		// std::vector<std::pair<Renderer *, Transform *>> culledBuckets[3];
		C3D_Mtx cameraMatrix[2];
		void updateMatrix(float iod);
		int screenwidth, screenheight; // only used when render texture is involved
	  public:
		float nearClip, farClip, focalLength = 2.f, fovY, height, width, aspectRatio;
		bool stereoEnabled, orthographic, highRes, active;
		uint32_t backgroundColour, cullingMask;
		iod_func iodMapFunc = [](float iod) { return iod * 0.2f; }; // default iod map function
		Camera(const void *args);
		~Camera();
	};
} // namespace ql