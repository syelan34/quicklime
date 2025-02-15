#ifndef HLSL_SUPPORT_INCLUDED
#define HLSL_SUPPORT_INCLUDED

// Define the underlying compiler being used.
#define QL_COMPILER_HLSL

#pragma warning (disable : 3205) // conversion of larger type to smaller
#pragma warning (disable : 3568) // unknown pragma ignored
#pragma warning (disable : 3571) // "pow(f,e) will not work for negative f"; however in majority of our calls to pow we know f is not negative
#pragma warning (disable : 3206) // implicit truncation of vector type

// swap fixed (nonexistent) to half
#define fixed half
#define fixed2 half2
#define fixed3 half3
#define fixed4 half4
#define fixed4x4 half4x4
#define fixed3x3 half3x3
#define fixed2x2 half2x2

#define QL_UV_STARTS_AT_TOP 1
#define QL_REVERSED_Z 1
#define QL_NEAR_CLIP_VALUE (1.0)

#define QL_INITIALIZE_OUTPUT(type,name) name = (type)0;

// HLSL attributes
#define QL_BRANCH    [branch]
#define QL_FLATTEN   [flatten]
#define QL_UNROLL    [unroll]
#define QL_LOOP      [loop]
#define QL_FASTOPT   [fastopt]

#endif // HLSL_SUPPORT_INCLUDED