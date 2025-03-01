#ifndef UNIFORMS_INCLUDED
#define UNIFORMS_INCLUDED

#include "HLSLSupport.cginc"

uniform row_major float4x4 ql_matrix_m    : register(c0);
uniform row_major float4x4 ql_matrix_im   : register(c4);
uniform row_major float4x4 ql_matrix_v    : register(c8);
uniform row_major float4x4 ql_matrix_iv   : register(c12);
uniform row_major float4x4 ql_matrix_p    : register(c16);
uniform row_major float4x4 ql_matrix_vp   : register(c20);

// first 4 lights
uniform float4 _LightColor[4]       : register(c24);
uniform float4 _LightPosition[4]    : register(c28);

// x = time since scene start
// y = sin(t)
// z = cos(t)
// w = dt
uniform float4 _Time : register(c32);
uniform float4 _ScreenParams : register(c33);

#define QL_MATRIX_MVP       mul(ql_matrix_vp, ql_matrix_m)r
#define QL_MATRIX_MV        mul(ql_matrix_v, ql_matrix_m)
#define QL_MATRIX_V         ql_matrix_v
#define QL_MATRIX_P         ql_matrix_p
#define QL_MATRIX_VP        ql_matrix_vp
#define QL_MATRIX_T_MV      transpose(QL_MATRIX_MV)
#define ql_ObjectToWorld    ql_matrix_m
#define ql_WorldToObject    ql_matrix_im
#define _WorldSpaceCameraPos float4(ql_matrix_iv[0].w, ql_matrix_iv[1].w, ql_matrix_iv[2].w, ql_matrix_iv[3].w)

#define QL_PI            3.14159265359f
#define QL_TWO_PI        6.28318530718f
#define QL_FOUR_PI       12.56637061436f
#define QL_INV_PI        0.31830988618f
#define QL_INV_TWO_PI    0.15915494309f
#define QL_INV_FOUR_PI   0.07957747155f
#define QL_HALF_PI       1.57079632679f
#define QL_INV_HALF_PI   0.636619772367f

#ifdef QL_COLORSPACE_GAMMA
#define ql_ColorSpaceGrey fixed4(0.5, 0.5, 0.5, 0.5)
#define ql_ColorSpaceDouble fixed4(2.0, 2.0, 2.0, 2.0)
#define ql_ColorSpaceDielectricSpec half4(0.220916301, 0.220916301, 0.220916301, 1.0 - 0.220916301)
#define ql_ColorSpaceLuminance half4(0.22, 0.707, 0.071, 0.0) // Legacy: alpha is set to 0.0 to specify gamma mode
#else // Linear values
#define ql_ColorSpaceGrey fixed4(0.214041144, 0.214041144, 0.214041144, 0.5)
#define ql_ColorSpaceDouble fixed4(4.59479380, 4.59479380, 4.59479380, 2.0)
#define ql_ColorSpaceDielectricSpec half4(0.04, 0.04, 0.04, 1.0 - 0.04) // standard dielectric reflectivity coef at incident angle (= 4%)
#define ql_ColorSpaceLuminance half4(0.0396819152, 0.458021790, 0.00609653955, 1.0) // Legacy: alpha is set to 1.0 to specify linear mode
#endif

#endif