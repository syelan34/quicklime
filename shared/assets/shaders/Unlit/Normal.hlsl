#include "UnityCG.cginc"

struct appdata_t {
	float4 vertex : POSITION;
	float2 texcoord : TEXCOORD0;
};

struct v2f {
	float4 vertex : POSITION;
	float2 texcoord : TEXCOORD0;
	half4 color : COLOR;
};

v2f main(appdata_t v) {
    v2f o;
    o.vertex = UnityObjectToClipPos(v.vertex);
	o.texcoord = v.texcoord;
	o.color = float4(1, 1, 1, 1);
}