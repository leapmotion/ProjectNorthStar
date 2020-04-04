Shader "Unlit/NorthStarDistortion"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {
        // No culling or depth
        Cull Off ZWrite Off ZTest Always

        Pass
        {
            CGPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata {
                float4 vertex : POSITION;
                float2 uv     : TEXCOORD0;
            };

            struct v2f {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;

            v2f vert (appdata v) {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                return o;
            }

            // Screen UV to Rectilinear Coefficients
            uniform float uv_to_rect_x[16];
            uniform float uv_to_rect_y[16];
            uniform float4x4 camera_matrix;
            uniform float4 offset_scaling;
            uniform float  is_left;

            // Evaluate a 2D polynomial from its coefficients
            float polyval2d(float X, float Y, float C[16]) {
              float X2 = X * X; float X3 = X2 * X;
              float Y2 = Y * Y; float Y3 = Y2 * Y;
              return (((C[ 0]     ) + (C[ 1]      * Y) + (C[ 2]      * Y2) + (C[ 3]      * Y3)) +
                      ((C[ 4] * X ) + (C[ 5] * X  * Y) + (C[ 6] * X  * Y2) + (C[ 7] * X  * Y3)) +
                      ((C[ 8] * X2) + (C[ 9] * X2 * Y) + (C[10] * X2 * Y2) + (C[11] * X2 * Y3)) +
                      ((C[12] * X3) + (C[13] * X3 * Y) + (C[14] * X3 * Y2) + (C[15] * X3 * Y3)));
            }

            float3 WorldToViewport(float4x4 camVP, float3 worldPoint) {
                float4 result;
                result.x = camVP[0][0] * worldPoint.x + camVP[0][1] * worldPoint.y + camVP[0][2] * worldPoint.z + camVP[0][3];
                result.y = camVP[1][0] * worldPoint.x + camVP[1][1] * worldPoint.y + camVP[1][2] * worldPoint.z + camVP[1][3];
                result.z = camVP[2][0] * worldPoint.x + camVP[2][1] * worldPoint.y + camVP[2][2] * worldPoint.z + camVP[2][3];
                float  w = camVP[3][0] * worldPoint.x + camVP[3][1] * worldPoint.y + camVP[3][2] * worldPoint.z + camVP[3][3];
                result.x /= w; result.y /= w;

                result.x = (result.x * 0.5 + 0.5);
                result.y = (result.y * 0.5 + 0.5);

                return result.xyz;
            }

            fixed4 frag(v2f i) : SV_Target {
                // Window Position/Scale Invariant Screen-space UVs...
                float2 uv = float2(( (i.uv.x + offset_scaling.x) * offset_scaling.z) ,
                                    ((i.uv.y - offset_scaling.y) * offset_scaling.w));
                uv = float2(uv.x, (uv.y + 0.5) * 2.0);
                uv.x -= 1.0-is_left;

                // Calculate the Rectilinear Coordinates from the UVs
                float3 rectilinear_coordinate = float3(polyval2d(1.0 - uv.x, uv.y, uv_to_rect_x),
                                                       polyval2d(1.0 - uv.x, uv.y, uv_to_rect_y), 1.0);
                
                // Project the Rectilinear Coordinate through this camera matrix
                float2 distorted_uv = WorldToViewport(camera_matrix, rectilinear_coordinate);

                // Sample the Camera Texture at this distorted coordinate
                return tex2D(_MainTex, distorted_uv); //fixed4(distorted_uv.x, distorted_uv.y, 0.0, 1.0);//fixed4(uv.x, uv.y, 0, 1.0);//fixed4(uv.x, uv.y, 0.0, 1.0);//
            }
            ENDCG
        }
    }
}
