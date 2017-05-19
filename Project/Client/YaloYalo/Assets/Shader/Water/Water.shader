Shader "Custom/Water" 
{
	Properties 
	{
		_TintColor("Main Color", Color) = (1,1,1,1)
		_MainTex("MainTex Map", 2D) = "white" {}
		_WaterBump("Water Bump", 2D) = "white" {}
		_TimeScale("Wave Time ", Range(0,1)) = 0.1
		_WaveScale("Wave Scale", Float) = 0.2
		_WaveFrec("Wave Scale", Float) = 1.0
	}

	SubShader 
	{
		Tags { "RenderType"="Opaque" }
		Cull Off
		LOD 300
		
		CGPROGRAM
		#pragma surface surf Standard Lambert vertex:vert // Physically based Standard lighting model, and enable shadows on all light types
		#pragma target 3.0 // Use shader model 3.0 target, to get nicer looking lighting
		#include "UnityCG.cginc"

		sampler2D _MainTex;
		sampler2D _WaterBump;

		fixed4	_TintColor;
		fixed	_TimeScale;
		fixed	_WaveScale;


		struct Input 
		{
			float2 uv_MainTex;
			INTERNAL_DATA
		};

		void vert (inout appdata_full i)
		{
			//fixed
			fixed FracTime = frac(_Time*_TimeScale);   //frac : surface effector 에 fractal noise 효과를 주는것

			fixed2 NoisUV = float2(i.texcoord.x + FracTime, i.texcoord.y + FracTime); //x 와 y쪽에 각각 frac적용 시간을 더합니다. 

			fixed3 bump = tex2Dlod(_WaterBump, float4(NoisUV, 0, 0)).rgb;

			fixed waterwave = ((bump.x + bump.y + bump.z) / 3 - 0.5) * _WaveScale;

			i.vertex.xyz = i.vertex.xyz + (normalize(i.normal.xyz) * waterwave); //vertex coordinate
			//i.vertex.xyz = i.vertex.xyz + waterwave;
		}

		void surf (Input IN, inout SurfaceOutputStandard o) 
		{
			half4 tex = tex2D(_MainTex, IN.uv_MainTex);

			o.Albedo = tex.rgb * _TintColor.rgb;
		}
		ENDCG
	}

	FallBack "Diffuse"

}
