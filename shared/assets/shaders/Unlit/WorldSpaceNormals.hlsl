// include file that contains UnityObjectToWorldNormal helper function
#include "../include/QuicklimeCG.cginc"

struct v2f {
    // we'll output world space normal as the colour directly
    half3 worldNormal : COLOR;
    float4 pos : SV_POSITION;
};

// vertex shader: takes object space normal as input too
v2f main(float4 vertex : POSITION, float3 normal : NORMAL)
{
    v2f o;
    o.pos = QuicklimeObjectToClipPos(vertex);
    // UnityCG.cginc file contains function to transform
    // normal from object to world space, use that
    o.worldNormal = QuicklimeObjectToWorldNormal(normal);
    return o;
}