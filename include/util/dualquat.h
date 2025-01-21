#pragma once

#include "c3d/maths.h"
#include <citro3d.h>
#include <cmath>

namespace ql {
	union DualQuat {
		C3D_FQuat q[2];
		struct {
			C3D_FQuat qr; // real component
			C3D_FQuat qd; // dual component
		};
	};
	union Dual {
		float c[2];
		struct {
			float r;
			float d;
		};
	}
	
	DualQuat DQ_Identity() {
		return {{
			Quat_New(0, 0, 0, 1),
			Quat_New(0, 0, 0, 0)
		}};
	}
	DualQuat DQ_fromPosRot(C3D_FQuat rotation, C3D_FVec position) {
		// qr = r
		// qd = 0.5(tr)
		return {{
			rotation,
			Quat_Scale(Quat_Multiply(rotation, position), 0.5)
		}};
	}
	void DQ_toPosRot(C3D_FQuat* outrotation, C3D_FVec* outposition, DualQuat dq) {
		if (outrotation == nullptr || outposition == nullptr) return;
		*outrotation = dq.qr;
		*outposition = Quat_Multiply(Quat_Scale(dq.qd, 2), Quat_Conjugate(dq.qr));
	}
	
	DualQuat DQ_Scale(DualQuat lhs, float scale) {
		return {{
			Quat_Scale(lhs.qr, scale),
			Quat_Scale(lhs.qd, scale)
		}};
	}
	
	DualQuat DQ_Add(DualQuat lhs, DualQuat rhs) {
		return {{
			Quat_Add(lhs.qr, rhs.qr),
			Quat_Add(lhs.qd, rhs.qd)
		}};
	}
	
	DualQuat DQ_Multiply(DualQuat lhs, DualQuat rhs) {
		// q1q2 = qr1qr2 + (qr1qd2 + qr2qd1)e
		return {{
			Quat_Multiply(lhs.qr, rhs.qr),
			Quat_Add(Quat_Multiply(lhs.qr, rhs.qd), Quat_Multiply(rhs.qr, lhs.qd))
		}};
	}
	
	float DQ_Dot(DualQuat lhs, DualQuat rhs) {
		return Quat_Dot(lhs.qr, rhs.qr);
	}
	
	DualQuat DQ_Conjugate(DualQuat dq) {
		// q* = qr* + (qd*)e
		return {{
			Quat_Conjugate(dq.qr),
			Quat_Conjugate(dq.qd)
		}};
	}
	
	Dual DQ_Magnitude(DualQuat dq) {
		// ||q||^2 = qq*
		//         = Sc(q)^2 + Vec(q)*Vec(q)
		//         = (qr0 + eqd0)^2 + Vec(q)*Vec(q)
		//         = |qr0| + Vec(q)*Vec(q)
		return {{
			
		}};
	}
	
	DualQuat DQ_SepLERP(float t, DualQuat a, DualQuat b) {
		C3D_FQuat _ra, _rb;
		C3D_FVec _ta, _tb;
		DQ_toPosRot(&_ra, &_ta, a);
		DQ_toPosRot(&_rb, &_tb, b);
		return DQ_fromPosRot(
			Quat_Multiply(_ra, Quat_Pow(Quat_Multiply(Quat_Conjugate(_ra), _rb), t)), 
			FVec4_Add(FVec4_Scale(_ta, 1-t), FVec4_Scale(_tb, t))
		);
	}
	DualQuat DQ_QLB(float t, DualQuat a, DualQuat b);
	DualQuat DQ_ScLERP(float t, DualQuat a, DualQuat b) {
		return DQ_Multiply(a, DQ_Power)
	}
	DualQuat DQ_SepLERP(float t, DualQuat a, DualQuat b);
}

/*
Dual quaternion math from here:
https://arxiv.org/pdf/2303.13395
*/