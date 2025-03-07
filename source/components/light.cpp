#include "light.h"
#include "lights.h"
#include "componentmanager.h"
#include "ql_assert.h"
#include "transform.h"
#include <climits>
#include "shared_unif_locations.h"

// LightLock Light::lock = LightLock();
namespace ql {
    namespace {
        enum Attn_Type { NONE, LINEAR, QUADRATIC };
        
        struct light_args {
            u32 flags;
            C3D_FVec position, color;
            Attn_Type attn_type;
            float attnparam0, attnparam1;
            float attn_bias, attn_scale;
        };
        // float distanceAttenLin(float d, float, float) {
        //     return 1.0f - d;
        // }
        // float distanceAttenQuad(float d, float linear, float quadratic) {
        //     return 1.0f / (1.0f + linear * d + quadratic * d * d);
        // }
    }
    Light::Light(std::weak_ptr<GameObject> owner, const void *data) : p(owner) {
        ASSERT(data != nullptr, "Invalid light constructor arg");
        
        light_args &args = *(light_args *)data;
        flags = args.flags;
        position = args.position;
        color = args.color;
        
        //TODO set light luts etc
    }
	Light &Light::operator=(ql::Light &&l) {
		p	= l.p;
		l.p = {};
		id = l.id;
		l.id = UINT_MAX;
		return *this;
	}
	
	Light::~Light() {
	   lights::freeLight(id);
	}

	void Light::setSelf(C3D_Mtx &view) {
	    ASSERT(!p.expired(), "Parent expired");
	    C3D_Mtx model = *p.lock()->getComponent<Transform>();
		C3D_Mtx modelView;
		Mtx_Multiply(&modelView, &model, &view);
		
		C3D_FVec transformedPosition = Mtx_MultiplyFVec4(&modelView, position);
		C3D_LightPosition(&lights::lights[id], &transformedPosition); // fine because it makes a copy
		C3D_LightColor(&lights::lights[id], color.x, color.y, color.z);
		
		if (id > 4) return;
		C3D_FVUnifSet(GPU_VERTEX_SHADER, shared_unifs::lightcolor_loc + id, color.x, color.y, color.z, 1.0f);
	    C3D_FVUnifSet(GPU_VERTEX_SHADER, shared_unifs::lightposition_loc + id, transformedPosition.x, transformedPosition.y, transformedPosition.z, 1.0f);
		
		// std::visit([&](auto &light) { light.set(modelView); }, internal_light);
	}
	
	void Light::setPosition(C3D_FVec& position) {
        this->position = position;
    }
    
    void Light::setEnabled(bool enabled) {
        C3D_LightEnable(&lights::lights[id], enabled);
    }
    
    void Light::setGeoFactorEnable(bool enabled, GeoFactorId id) {
        C3D_LightGeoFactor(&lights::lights[id], id, enabled);
    }
    
    void Light::set2SideDiffuseEnable(bool enabled) {
        C3D_LightTwoSideDiffuse(&lights::lights[id], enabled);
    }
    
    void Light::setShadowEnabled(bool enabled) {
        C3D_LightShadowEnable(&lights::lights[id], enabled);
    }
    
    void Light::setDistAttenEnabled(bool enabled) {
        C3D_LightDistAttnEnable(&lights::lights[id], enabled);
    }
    
    void Light::setDirectional(LightType type) {
        switch (type) {
            case LightType_POSITIONAL:
                lights::lights[id].conf.config |= BIT(0);
                break;
            case LightType_DIRECTIONAL:
                lights::lights[id].conf.config &= ~BIT(0);
                break;
        }
    	lights::lights[id].flags |= C3DF_Light_Dirty;
    }
	
	
	COMPONENT_REGISTER(Light);
} // namespace ql