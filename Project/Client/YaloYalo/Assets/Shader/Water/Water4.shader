Shader "Custom/Water4"
{
	Properties
	{
		//_TintColor("Main Color", Color) = (1,1,1,1)
		_MainTex("MainTex", 2D) = "white" {}
		_BumpTex("Water Bump", 2D) = "white" {}
		//Bump
		_Bumpiness("Bumpiness", Float) = 2.0
		_Speed("Speed", Float) = 1.0
		//Wave
		_WaveSpeed("Wave Speed", Range(0.1, 80)) = 2.5
		_Frequency("Wave Frequency", Range(0, 5)) = 2.5
		_Amplitude("Wave Amplitude", Range(-1, 1)) = 0.7
	}

	SubShader
	{
		Tags{ "RenderType" = "Opaque" }
		Cull Off
		LOD 300

		CGPROGRAM
		#pragma surface surf Standard Lambert vertex:vert	// Physically based Standard lighting model, and enable shadows on all light types
		#pragma target 3.0									// Use shader model 3.0 target, to get nicer looking lighting
		//#include "UnityCG.cginc"

		sampler2D	_MainTex;
		sampler2D	_BumpTex;

		float		_Bumpiness;
		float		_Speed;
	

		float _WaveSpeed;
		float _Frequency;
		float _Amplitude;

		struct Input
		{
			float2 uv_MainTex;
			INTERNAL_DATA
		};
		
		void vert(inout appdata_full v, out Input o)
		{
			UNITY_INITIALIZE_OUTPUT(Input, o);

			float time = _Time.y * _WaveSpeed;

			float waveValueA = sin(time + v.vertex.x * _Frequency) * _Amplitude;

			float waveValueB = cos(time + v.vertex.y * _Frequency) * _Amplitude;

			float waveValueC = sin(time*1.5 + (v.vertex.z + v.vertex.y) * _Frequency) * _Amplitude;

			float waveValueD = cos(time*1.5 + (v.vertex.z + v.vertex.y) * _Frequency) * _Amplitude;

			float waveValueE = sin(time*0.5 + (v.vertex.z + v.vertex.x) * _Frequency*0.3) * _Amplitude;

			float waveValueF = cos(time*0.5 + (v.vertex.z + v.vertex.x) * _Frequency*0.3) * _Amplitude;


			float3 VMove = float3(v.vertex.x + waveValueA, v.vertex.y + waveValueB, v.vertex.z);

			VMove = VMove + float3(v.vertex.x + waveValueC, v.vertex.y + waveValueD, v.vertex.z);

			VMove = VMove + float3(v.vertex.x + waveValueF, v.vertex.y + waveValueE, v.vertex.z);

			v.vertex.x = VMove.x;
		}
		
		void surf(Input IN, inout SurfaceOutputStandard o)
		{
			float2 uv = float2(IN.uv_MainTex.x + _Time.y *_Speed, IN.uv_MainTex.y); //+_Time * _Speed
		
			float3 normal;
		
			normal = tex2D(_BumpTex, uv);
		
			half4 tex = tex2D(_MainTex, IN.uv_MainTex.xy + _Bumpiness * normal.xy);
		
			o.Albedo = tex.rgb; //* _TintColor.rgb;
		}

	ENDCG

	}
		FallBack "Diffuse"
}
