#ifndef TEST_CG_INCLUDED
#define TEST_CG_INCLUDED

#define QL_PI            3.14159265359f
#define QL_TWO_PI        6.28318530718f
#define QL_FOUR_PI       12.56637061436f
#define QL_INV_PI        0.31830988618f
#define QL_INV_TWO_PI    0.15915494309f
#define QL_INV_FOUR_PI   0.07957747155f
#define QL_HALF_PI       1.57079632679f
#define QL_INV_HALF_PI   0.636619772367f

#include "QuicklimeShaderVariables.cginc"
// #include "QuicklimeInstancing.cginc"

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

#define SCALED_NORMAL v.normal


struct appdata_base {
    float4 vertex : POSITION;
    float3 normal : NORMAL;
    float4 texcoord : TEXCOORD0;
};

struct appdata_tan {
    float4 vertex : POSITION;
    float4 tangent : TANGENT;
    float3 normal : NORMAL;
    float4 texcoord : TEXCOORD0;
};

struct appdata_full {
    float4 vertex : POSITION;
    float4 tangent : TANGENT;
    float3 normal : NORMAL;
    float4 texcoord : TEXCOORD0;
    float4 texcoord1 : TEXCOORD1;
    float4 texcoord2 : TEXCOORD2;
    float4 color : COLOR;
};

// Legacy for compatibility with existing shaders
inline bool IsGammaSpace()
{
    #ifdef QL_COLORSPACE_GAMMA
    return true;
    #else
        return false;
    #endif
}

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
    return _WorldSpaceLightPos0.xyz - worldPos;
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
    float3 objSpaceLightPos = mul(ql_WorldToObject, _WorldSpaceLightPos0).xyz;
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



// Used in ForwardBase pass: Calculates diffuse lighting from 4 point lights, with data packed in a special way.
float3 Shade4PointLights (
    float4 lightPosX, float4 lightPosY, float4 lightPosZ,
    float3 lightColor0, float3 lightColor1, float3 lightColor2, float3 lightColor3,
    float4 lightAttenSq,
    float3 pos, float3 normal)
{
    // to light vectors
    float4 toLightX = lightPosX - pos.x;
    float4 toLightY = lightPosY - pos.y;
    float4 toLightZ = lightPosZ - pos.z;
    // squared lengths
    float4 lengthSq = 0;
    lengthSq += toLightX * toLightX;
    lengthSq += toLightY * toLightY;
    lengthSq += toLightZ * toLightZ;
    // don't produce NaNs if some vertex position overlaps with the light
    lengthSq = max(lengthSq, 0.000001);

    // NdotL
    float4 ndotl = 0;
    ndotl += toLightX * normal.x;
    ndotl += toLightY * normal.y;
    ndotl += toLightZ * normal.z;
    // correct NdotL
    float4 corr = rsqrt(lengthSq);
    ndotl = max (float4(0,0,0,0), ndotl * corr);
    // attenuation
    float4 atten = 1.0 / (1.0 + lengthSq * lightAttenSq);
    float4 diff = ndotl * atten;
    // final color
    float3 col = 0;
    col += lightColor0 * diff.x;
    col += lightColor1 * diff.y;
    col += lightColor2 * diff.z;
    col += lightColor3 * diff.w;
    return col;
}

// Used in Vertex pass: Calculates diffuse lighting from lightCount lights. Specifying true to spotLight is more expensive
// to calculate but lights are treated as spot lights otherwise they are treated as point lights.
float3 ShadeVertexLightsFull (float4 vertex, float3 normal, int lightCount, bool spotLight)
{
    float3 viewpos = QuicklimeObjectToViewPos (vertex);
    float3 viewN = normalize (mul ((float3x3)QL_MATRIX_IT_MV, normal));

    float3 lightColor = QL_LIGHTMODEL_AMBIENT.xyz;
    for (int i = 0; i < lightCount; i++) {
        float3 toLight = ql_LightPosition[i].xyz - viewpos.xyz * ql_LightPosition[i].w;
        float lengthSq = dot(toLight, toLight);

        // don't produce NaNs if some vertex position overlaps with the light
        lengthSq = max(lengthSq, 0.000001);

        toLight *= rsqrt(lengthSq);

        float atten = 1.0 / (1.0 + lengthSq * ql_LightAtten[i].z);
        if (spotLight)
        {
            float rho = max (0, dot(toLight, ql_SpotDirection[i].xyz));
            float spotAtt = (rho - ql_LightAtten[i].x) * ql_LightAtten[i].y;
            atten *= saturate(spotAtt);
        }

        float diff = max (0, dot (viewN, toLight));
        lightColor += ql_LightColor[i].rgb * (diff * atten);
    }
    return lightColor;
}

float3 ShadeVertexLights (float4 vertex, float3 normal)
{
    return ShadeVertexLightsFull (vertex, normal, 4, false);
}

// normal should be normalized, w=1.0
half3 SHEvalLinearL0L1 (half4 normal)
{
    half3 x;

    // Linear (L1) + constant (L0) polynomial terms
    x.r = dot(ql_SHAr,normal);
    x.g = dot(ql_SHAg,normal);
    x.b = dot(ql_SHAb,normal);

    return x;
}

// normal should be normalized, w=1.0
half3 SHEvalLinearL2 (half4 normal)
{
    half3 x1, x2;
    // 4 of the quadratic (L2) polynomials
    half4 vB = normal.xyzz * normal.yzzx;
    x1.r = dot(ql_SHBr,vB);
    x1.g = dot(ql_SHBg,vB);
    x1.b = dot(ql_SHBb,vB);

    // Final (5th) quadratic (L2) polynomial
    half vC = normal.x*normal.x - normal.y*normal.y;
    x2 = ql_SHC.rgb * vC;

    return x1 + x2;
}

// normal should be normalized, w=1.0
// output in active color space
half3 ShadeSH9 (half4 normal)
{
    // Linear + constant polynomial terms
    half3 res = SHEvalLinearL0L1 (normal);

    // Quadratic polynomials
    res += SHEvalLinearL2 (normal);

#   ifdef QL_COLORSPACE_GAMMA
        res = LinearToGammaSpace (res);
#   endif

    return res;
}

// OBSOLETE: for backwards compatibility with 5.0
half3 ShadeSH3Order(half4 normal)
{
    // Quadratic polynomials
    half3 res = SHEvalLinearL2 (normal);

#   ifdef QL_COLORSPACE_GAMMA
        res = LinearToGammaSpace (res);
#   endif

    return res;
}

// normal should be normalized, w=1.0
half3 ShadeSH12Order (half4 normal)
{
    // Linear + constant polynomial terms
    half3 res = SHEvalLinearL0L1 (normal);

#   ifdef QL_COLORSPACE_GAMMA
        res = LinearToGammaSpace (res);
#   endif

    return res;
}

// Transforms 2D UV by scale/bias property
#define TRANSFORM_TEX(tex,name) (tex.xy * name##_ST.xy + name##_ST.zw)

// Deprecated. Used to transform 4D UV by a fixed function texture matrix. Now just returns the passed UV.
#define TRANSFORM_UV(idx) v.texcoord.xy



struct v2f_vertex_lit {
    float2 uv   : TEXCOORD0;
    fixed4 diff : COLOR0;
    fixed4 spec : COLOR1;
};

inline fixed4 VertexLight( v2f_vertex_lit i, sampler2D mainTex )
{
    fixed4 texcol = tex2D( mainTex, i.uv );
    fixed4 c;
    c.xyz = ( texcol.xyz * i.diff.xyz + i.spec.xyz * texcol.a );
    c.w = texcol.w * i.diff.w;
    return c;
}


// Calculates UV offset for parallax bump mapping
inline float2 ParallaxOffset( half h, half height, half3 viewDir )
{
    h = h * height - height/2.0;
    float3 v = normalize(viewDir);
    v.z += 0.42;
    return h * (v.xy / v.z);
}

// Converts color to luminance (grayscale)
inline half Luminance(half3 rgb)
{
    return dot(rgb, ql_ColorSpaceLuminance.rgb);
}

// Convert rgb to luminance
// with rgb in linear space with sRGB primaries and D65 white point
half LinearRgbToLuminance(half3 linearRgb)
{
    return dot(linearRgb, half3(0.2126729f,  0.7151522f, 0.0721750f));
}


// Decodes HDR textures
// handles dLDR, RGBM formats, Compressed(BC6H, BC1), and Uncompressed(RGBAHalf, RGBA32)
inline half3 DecodeHDR (half4 data, half4 decodeInstructions)
{
    // Take into account texture alpha if decodeInstructions.w is true(the alpha value affects the RGB channels)
    half alpha = decodeInstructions.w * (data.a - 1.0) + 1.0;

    // If Linear mode is not supported we can skip exponent part
    #if defined(QL_COLORSPACE_GAMMA)
        return (decodeInstructions.x * alpha) * data.rgb;
    #else
    #   if defined(QL_USE_NATIVE_HDR)
            return decodeInstructions.x * data.rgb; // Multiplier for future HDRI relative to absolute conversion.
    #   else
            return (decodeInstructions.x * pow(alpha, decodeInstructions.y)) * data.rgb;
    #   endif
    #endif
}


// Encoding/decoding [0..1) floats into 8 bit/channel RGBA. Note that 1.0 will not be encoded properly.
inline float4 EncodeFloatRGBA( float v )
{
    float4 kEncodeMul = float4(1.0, 255.0, 65025.0, 16581375.0);
    float kEncodeBit = 1.0/255.0;
    float4 enc = kEncodeMul * v;
    enc = frac (enc);
    enc -= enc.yzww * kEncodeBit;
    return enc;
}
inline float DecodeFloatRGBA( float4 enc )
{
    float4 kDecodeDot = float4(1.0, 1/255.0, 1/65025.0, 1/16581375.0);
    return dot( enc, kDecodeDot );
}

// Encoding/decoding [0..1) floats into 8 bit/channel RG. Note that 1.0 will not be encoded properly.
inline float2 EncodeFloatRG( float v )
{
    float2 kEncodeMul = float2(1.0, 255.0);
    float kEncodeBit = 1.0/255.0;
    float2 enc = kEncodeMul * v;
    enc = frac (enc);
    enc.x -= enc.y * kEncodeBit;
    return enc;
}
inline float DecodeFloatRG( float2 enc )
{
    float2 kDecodeDot = float2(1.0, 1/255.0);
    return dot( enc, kDecodeDot );
}


// Encoding/decoding view space normals into 2D 0..1 vector
inline float2 EncodeViewNormalStereo( float3 n )
{
    float kScale = 1.7777;
    float2 enc;
    enc = n.xy / (n.z+1);
    enc /= kScale;
    enc = enc*0.5+0.5;
    return enc;
}
inline float3 DecodeViewNormalStereo( float4 enc4 )
{
    float kScale = 1.7777;
    float3 nn = enc4.xyz*float3(2*kScale,2*kScale,0) + float3(-kScale,-kScale,1);
    float g = 2.0 / dot(nn.xyz,nn.xyz);
    float3 n;
    n.xy = g*nn.xy;
    n.z = g-1;
    return n;
}

inline float4 EncodeDepthNormal( float depth, float3 normal )
{
    float4 enc;
    enc.xy = EncodeViewNormalStereo (normal);
    enc.zw = EncodeFloatRG (depth);
    return enc;
}

inline void DecodeDepthNormal( float4 enc, out float depth, out float3 normal )
{
    depth = DecodeFloatRG (enc.zw);
    normal = DecodeViewNormalStereo (enc);
}

inline fixed3 UnpackNormalDXT5nm (fixed4 packednormal)
{
    fixed3 normal;
    normal.xy = packednormal.wy * 2 - 1;
    normal.z = sqrt(1 - saturate(dot(normal.xy, normal.xy)));
    return normal;
}

inline fixed3 UnpackNormal(fixed4 packednormal)
{
#if defined(QL_NO_DXT5nm)
    return packednormal.xyz * 2 - 1;
#else
    return UnpackNormalDXT5nm(packednormal);
#endif
}


// Z buffer to linear 0..1 depth
inline float Linear01Depth( float z )
{
    return 1.0 / (_ZBufferParams.x * z + _ZBufferParams.y);
}
// Z buffer to linear depth
inline float LinearEyeDepth( float z )
{
    return 1.0 / (_ZBufferParams.z * z + _ZBufferParams.w);
}


float2 TransformStereoScreenSpaceTex(float2 uv, float w)
{
    float4 scaleOffset = ql_StereoScaleOffset[ql_StereoEyeIndex];
    return uv.xy * scaleOffset.xy + scaleOffset.zw * w;
}

#ifdef QL_SINGLE_PASS_STEREO
inline float2 QuicklimeStereoScreenSpaceUVAdjustInternal(float2 uv, float4 scaleAndOffset)
{
    return uv.xy * scaleAndOffset.xy + scaleAndOffset.zw;
}

inline float4 QuicklimeStereoScreenSpaceUVAdjustInternal(float4 uv, float4 scaleAndOffset)
{
    return float4(QuicklimeStereoScreenSpaceUVAdjustInternal(uv.xy, scaleAndOffset), QuicklimeStereoScreenSpaceUVAdjustInternal(uv.zw, scaleAndOffset));
}

#define QuicklimeStereoScreenSpaceUVAdjust(x, y) QuicklimeStereoScreenSpaceUVAdjustInternal(x, y)

inline float2 QuicklimeStereoTransformScreenSpaceTex(float2 uv)
{
    return TransformStereoScreenSpaceTex(saturate(uv), 1.0);
}

inline float4 QuicklimeStereoTransformScreenSpaceTex(float4 uv)
{
    return float4(QuicklimeStereoTransformScreenSpaceTex(uv.xy), QuicklimeStereoTransformScreenSpaceTex(uv.zw));
}
#else
#define QuicklimeStereoScreenSpaceUVAdjust(x, y) x
#define QuicklimeStereoTransformScreenSpaceTex(uv) uv
#endif

// Depth render texture helpers
#define DECODE_EYEDEPTH(i) LinearEyeDepth(i)
#define COMPUTE_EYEDEPTH(o) o = -QuicklimeObjectToViewPos( v.vertex ).z
#define COMPUTE_DEPTH_01 -(QuicklimeObjectToViewPos( v.vertex ).z * _ProjectionParams.w)
#define COMPUTE_VIEW_NORMAL normalize(mul((float3x3)QL_MATRIX_IT_MV, v.normal))

// Helpers used in image effects. Most image effects use the same
// minimal vertex shader (vert_img).

struct appdata_img
{
    float4 vertex : POSITION;
    half2 texcoord : TEXCOORD0;
};

struct v2f_img
{
    float4 pos : SV_POSITION;
    half2 uv : TEXCOORD0;
};

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

// Projected screen position helpers
#define V2F_SCREEN_TYPE float4

inline float4 ComputeNonStereoScreenPos(float4 pos) {
    float4 o = pos * 0.5f;
    o.xy = float2(o.x, o.y*_ProjectionParams.x) + o.w;
    o.zw = pos.zw;
    return o;
}

inline float4 ComputeScreenPos(float4 pos) {
    float4 o = ComputeNonStereoScreenPos(pos);
#if defined(QL_SINGLE_PASS_STEREO) || defined(STEREO_INSTANCING_ON)
    o.xy = TransformStereoScreenSpaceTex(o.xy, pos.w);
#endif
    return o;
}

inline float4 ComputeGrabScreenPos (float4 pos) {
    #if QL_UV_STARTS_AT_TOP
    float scale = -1.0;
    #else
    float scale = 1.0;
    #endif
    float4 o = pos * 0.5f;
    o.xy = float2(o.x, o.y*scale) + o.w;
#ifdef QL_SINGLE_PASS_STEREO
    o.xy = TransformStereoScreenSpaceTex(o.xy, pos.w);
#endif
    o.zw = pos.zw;
    return o;
}

// snaps post-transformed position to screen pixels
inline float4 QuicklimePixelSnap (float4 pos)
{
    float2 hpc = _ScreenParams.xy * 0.5f;
    float2 pixelPos = round ((pos.xy / pos.w) * hpc);
    pos.xy = pixelPos / hpc * pos.w + float2(0.5f, 0.5f);
    return pos;
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

#endif // QL_CG_INCLUDED