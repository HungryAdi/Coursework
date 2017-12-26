Shader "Unlit/BlobTex"
{
    Properties
    {
        _MainTex("Texture", 2D) = "white" {}
        _Color("Color", Color) = (1, 1, 1, 1)
    }
        SubShader
        {
            Tags { "RenderType" = "Opaque" }
            ZWrite On
			Lighting Off
            Blend SrcAlpha OneMinusSrcAlpha
			AlphaToMask On
            Pass
            {
                CGPROGRAM
                #pragma vertex vert
                #pragma fragment frag
                #pragma target 3.5
                #include "UnityCG.cginc"

                struct appdata {
                    float4 vertex : POSITION;
                    float2 uv : TEXCOORD0;
                    uint id : SV_VertexID;
                };

                struct v2f {
                    float4 vertex : SV_POSITION;
                    float2 uv : TEXCOORD0;
                };

                sampler2D _MainTex;
                float4 _MainTex_ST;
                float4 _Color;

                // mesh size (36) * num control points (10)
                // but leaving some extra incase its changed
                float4 _Offsets[1024];  // mesh size * control points should be < 1024 or need to change it here
                float4 _Controls[64];   // support up to 64 control points
                float _ControlCount;     // how many control points ur using (has to be float cuz material prop block cant do int i guess????) so just floor it lol

                v2f vert(appdata v) {
                    v2f o;
                    v.vertex = float4(0,0,0,0);
                    int cp = floor(_ControlCount);
                    for (int i = 0; i < cp; ++i) {
                        float4 off = _Offsets[v.id*cp + i];
                        v.vertex += float4(off.w * (_Controls[i].xyz + off.xyz),0);
                    }

                    o.vertex = UnityObjectToClipPos(v.vertex);
                    o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                    return o;
                }

                fixed4 frag(v2f i) : SV_Target
                {
                    // sample the texture
                    fixed4 col = tex2D(_MainTex, i.uv) * _Color;
                    return col;
                }
                ENDCG
            }
        }
}
