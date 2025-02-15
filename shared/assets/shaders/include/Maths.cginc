#ifndef MATHS_INCLUDED
#define MATHS_INCLUDED

#include "QuicklimeShaderVariables.cginc"
#include "Structs.cginc"

inline float GammaToLinearSpaceExact (float value)
{
    if (value <= 0.04045F)
        return value / 12.92F;
    else if (value < 1.0F)
        return pow((value + 0.055F)/1.055F, 2.4F);
    else
        return pow(value, 2.2F);
}

inline half3 GammaToLinearSpace (half3 sRGB)
{
    // Approximate version from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return sRGB * (sRGB * (sRGB * 0.305306011h + 0.682171111h) + 0.012522878h);

    // Precise version, useful for debugging.
    //return half3(GammaToLinearSpaceExact(sRGB.r), GammaToLinearSpaceExact(sRGB.g), GammaToLinearSpaceExact(sRGB.b));
}

inline float LinearToGammaSpaceExact (float value)
{
    if (value <= 0.0F)
        return 0.0F;
    else if (value <= 0.0031308F)
        return 12.92F * value;
    else if (value < 1.0F)
        return 1.055F * pow(value, 0.4166667F) - 0.055F;
    else
        return pow(value, 0.45454545F);
}

inline half3 LinearToGammaSpace (half3 linRGB)
{
    linRGB = max(linRGB, half3(0.h, 0.h, 0.h));
    // An almost-perfect approximation from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return max(1.055h * pow(linRGB, 0.416666667h) - 0.055h, 0.h);

    // Exact version, useful for debugging.
    //return half3(LinearToGammaSpaceExact(linRGB.r), LinearToGammaSpaceExact(linRGB.g), LinearToGammaSpaceExact(linRGB.b));
}

inline float4x4 MatrixInverse(float4x4 m) {
    
}

// Tranforms position from object to homogenous space
inline float4 QuicklimeObjectToClipPos( in float3 pos )
{
    return mul(QL_MATRIX_VP, mul(ql_ObjectToWorld, float4(pos, 1.0)));
}
inline float4 QuicklimeObjectToClipPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
    return QuicklimeObjectToClipPos(pos.xyz);
}


// Tranforms position from world to homogenous space
inline float4 QuicklimeWorldToClipPos( in float3 pos )
{
    return mul(QL_MATRIX_VP, float4(pos, 1.0));
}

// Tranforms position from view to homogenous space
inline float4 QuicklimeViewToClipPos( in float3 pos )
{
    return mul(QL_MATRIX_P, float4(pos, 1.0));
}

// Tranforms position from object to camera space
inline float3 QuicklimeObjectToViewPos( in float3 pos )
{
    return mul(QL_MATRIX_V, mul(ql_ObjectToWorld, float4(pos, 1.0))).xyz;
}
inline float3 QuicklimeObjectToViewPos(float4 pos) // overload for float4; avoids "implicit truncation" warning for existing shaders
{
    return QuicklimeObjectToViewPos(pos.xyz);
}

// Tranforms position from world to camera space
inline float3 QuicklimeWorldToViewPos( in float3 pos )
{
    return mul(QL_MATRIX_V, float4(pos, 1.0)).xyz;
}

// Transforms direction from object to world space
inline float3 QuicklimeObjectToWorldDir( in float3 dir )
{
    return normalize(mul((float3x3)ql_ObjectToWorld, dir));
}

// Transforms direction from world to object space
inline float3 QuicklimeWorldToObjectDir( in float3 dir )
{
    return normalize(mul((float3x3)ql_WorldToObject, dir));
}

// Transforms normal from object to world space
inline float3 QuicklimeObjectToWorldNormal( in float3 norm )
{
    return QuicklimeObjectToWorldDir(norm);
}

// Computes world space light direction, from world space position
inline float3 QuicklimeWorldSpaceLightDir( in float3 worldPos )
{
    return _LightPosition[0].xyz - worldPos;
}

// Computes world space light direction, from object space position
// *Legacy* Please use QuicklimeWorldSpaceLightDir instead
inline float3 WorldSpaceLightDir( in float4 localPos )
{
    float3 worldPos = mul(ql_ObjectToWorld, localPos).xyz;
    return QuicklimeWorldSpaceLightDir(worldPos);
}

// Computes object space light direction
inline float3 ObjSpaceLightDir( in float4 v )
{
    float3 objSpaceLightPos = mul(ql_WorldToObject, _LightPosition[0]).xyz;
    return objSpaceLightPos.xyz - v.xyz;
}

// Computes world space view direction, from object space position
inline float3 QuicklimeWorldSpaceViewDir( in float3 worldPos )
{
    return _WorldSpaceCameraPos.xyz - worldPos;
}

// Computes world space view direction, from object space position
// *Legacy* Please use QuicklimeWorldSpaceViewDir instead
inline float3 WorldSpaceViewDir( in float4 localPos )
{
    float3 worldPos = mul(ql_ObjectToWorld, localPos).xyz;
    return QuicklimeWorldSpaceViewDir(worldPos);
}

// Computes object space view direction
inline float3 ObjSpaceViewDir( in float4 v )
{
    float3 objSpaceCameraPos = mul(ql_WorldToObject, float4(_WorldSpaceCameraPos.xyz, 1)).xyz;
    return objSpaceCameraPos - v.xyz;
}

// Declares 3x3 matrix 'rotation', filled with tangent space basis
#define TANGENT_SPACE_ROTATION \
    float3 binormal = cross( normalize(v.normal), normalize(v.tangent.xyz) ) * v.tangent.w; \
    float3x3 rotation = float3x3( v.tangent.xyz, binormal, v.normal )

// Calculates UV offset for parallax bump mapping
inline float2 ParallaxOffset( half h, half height, half3 viewDir )
{
    h = h * height - height/2.0;
    float3 v = normalize(viewDir);
    v.z += 0.42;
    return h * (v.xy / v.z);
}

float2 MultiplyUV (float4x4 mat, float2 inUV) {
    float4 temp = float4 (inUV.x, inUV.y, 0, 0);
    temp = mul (mat, temp);
    return temp.xy;
}

v2f_img vert_img( appdata_img v )
{
    v2f_img o;
    o.pos = QuicklimeObjectToClipPos (v.vertex);
    o.uv = v.texcoord;
    return o;
}

inline float2 TransformViewToProjection (float2 v) {
    return mul((float2x2)QL_MATRIX_P, v);
}

inline float3 TransformViewToProjection (float3 v) {
    return mul((float3x3)QL_MATRIX_P, v);
}

// ------------------------------------------------------------------
//  Alpha helper

#define QL_OPAQUE_ALPHA(outputAlpha) outputAlpha = 1.0

#endif