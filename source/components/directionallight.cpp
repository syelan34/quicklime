#include "directionallight.h"

namespace ql {
    namespace {
        struct Directionallight_args {
            C3D_FVec direction;
        };
    } // namespace
    DirectionalLight::DirectionalLight(const void *data) {(void)data;}
    DirectionalLight::DirectionalLight(DirectionalLight &&l) {}
    DirectionalLight &DirectionalLight::operator=(DirectionalLight &&l) {return *this;}
    void DirectionalLight::set(C3D_Mtx &modelView) {
        (void)modelView;
    }
} // namespace ql