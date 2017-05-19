Shader "Custom/Water3"
{
	Properties
	{
		_TintColor("Main Color", Color) = (1,1,1,1)
		_MainTex("MainTex", 2D) = "white" {}
		_BumpTex("Water Bump", 2D) = "white" {}

		_Bumpiness("Bumpiness", Float) = 2.0
		_Speed("Speed", Float) = 1.0
	}

	SubShader
	{
		Tags{ "RenderType" = "Opaque" }
		Cull Off
		LOD 300

		CGPROGRAM
		#pragma surface surf Standard Lambert //vertex:vert	// Physically based Standard lighting model, and enable shadows on all light types
		#pragma target 3.0									// Use shader model 3.0 target, to get nicer looking lighting
		//#include "UnityCG.cginc"

		float4		_TintColor;
		sampler2D	_MainTex;
		sampler2D	_BumpTex;

		float		_Bumpiness;
		float		_Speed;

		struct Input
		{
			float2 uv_MainTex;
			INTERNAL_DATA
		};

		//void vert (inout appdata_full i)
		//void vert(inout appdata_full v, out Input o)
		//{
		//	
		//
		//}

		void surf(Input IN, inout SurfaceOutputStandard o)
		{
			float2 uv = float2( IN.uv_MainTex.x + _Time.y *_Speed, IN.uv_MainTex.y ); //+_Time * _Speed

			float3 normal;

			normal = tex2D(_BumpTex, uv);
				
			half4 tex = tex2D(_MainTex, IN.uv_MainTex.xy + _Bumpiness * normal.xy);

			o.Albedo = tex.rgb * _TintColor.rgb;
		}

	ENDCG

	}
		FallBack "Diffuse"
}
