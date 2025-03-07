#pragma once

#include "lights.h"
#include <citro3d.h>
#include <memory>

namespace ql {
	class GameObject;

	enum LightType {
		LightType_POSITIONAL,
		LightType_DIRECTIONAL,
	};
	
	enum GeoFactorId {
	    FACTOR0 = BIT(0), 
		FACTOR1 = BIT(1)
	};

	class Light {
	    friend class Camera;
		std::weak_ptr<GameObject> p;
		// std::variant<DummyLight, PointLight, DirectionalLight> internal_light;
		// LightType t;
		
		lights::light_id id;
		u32 flags;
		C3D_FVec position, color;
		
		/*
		Shared attribs:
		- light id
		- enabled
		- geo factor
		- two side diffuse
		- position
		- color
		- shadow enable
		- distance attenuation
		- distance attenuation lut
	    */
		
		void setSelf(C3D_Mtx &modelView);

	  public:
		Light(std::weak_ptr<GameObject> owner, const void *data);
		Light &operator=(Light &&);
		~Light();
		void setPosition(C3D_FVec& position);
		void setEnabled(bool enabled);
		void setGeoFactorEnable(bool enabled, GeoFactorId id);
		void set2SideDiffuseEnable(bool enabled);
		void setShadowEnabled(bool enabled);
		void setDistAttenEnabled(bool enabled);
		void setDirectional(LightType);
		void setDistAttenLut(C3D_LightLutFuncDA func);
		void setSpotlightEnabled(bool enabled);
		void setSpotlightLut(C3D_LightLutFunc func);
		void setSpotlightAngle(float angle);
		void setSpotlightDirection(C3D_FVec& direction);
	};
} // namespace ql
