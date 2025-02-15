#ifndef STRUCTS_INCLUDED
#define STRUCTS_INCLUDED

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

struct v2f_vertex_lit {
    float2 uv   : TEXCOORD0;
    fixed4 diff : COLOR0;
    fixed4 spec : COLOR1;
};

struct v2f_img
{
    float4 pos : SV_POSITION;
    half2 uv : TEXCOORD0;
};

struct appdata_img
{
    float4 vertex : POSITION;
    half2 texcoord : TEXCOORD0;
};


#endif