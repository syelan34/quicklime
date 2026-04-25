#include "scene.h"
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <memory>

namespace ql::systems {
    class Graphics {
        static C3D_RenderTarget *target[4];
        public:
        static void Init();
        static void Exit();
        static void DrawScene(std::unique_ptr<Scene>);
    };
}