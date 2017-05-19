Shader "Custom/Water2"
{
	Properties
	{
		_TintColor("Main Color", Color) = (1,1,1,1)
		_MainTex("MainTex", 2D) = "white" {}

		//_WaterBump("Water Bump", 2D) = "white" {}
		//_TimeScale("Wave Time ", Range(0,1)) = 0.1
		//_WaveScale("Wave Scale", Float) = 0.2

		_Speed("Wave Speed", Range(0.1, 80)) = 5
		_Frequency("Wave Frequency", Range(0, 5)) = 2
		_Amplitude("Wave Amplitude", Range(-1, 1)) = 1
	}

	SubShader
	{
		Tags{ "RenderType" = "Opaque" }
		Cull Off
		LOD 300

		CGPROGRAM
		#pragma surface surf Standard Lambert vertex:vert	// Physically based Standard lighting model, and enable shadows on all light types
		#pragma target 3.0									// Use shader model 3.0 target, to get nicer looking lighting
		#include "UnityCG.cginc"

		sampler2D _MainTex;

		fixed4	_TintColor;
		fixed	_TimeScale;
		fixed	_WaveScale;
		
		float _Speed;
		float _Frequency;
		float _Amplitude;

		struct Input
		{
			float2 uv_MainTex;
			INTERNAL_DATA
		};
	
		//void vert (inout appdata_full i)
		void vert(inout appdata_full v, out Input o)
		{
			//fixed FracTime = frac(_Time * _TimeScale);		
			//fixed FracX = i.texcoord.x + FracTime;
			//fixed2 NoisUV = float2(i.texcoord.x + FracTime, i.texcoord.y + FracTime);
			//fixed waterwave = ((FracX + i.vertex.y + i.vertex.z) / 3 - 0.5) * _WaveScale;
			//i.vertex.y = sin(i.vertex.x + FracTime * 100) ;


			//UNITY_INITIALIZE_OUTPUT(Input, o);
			//
			//float time = _Time * _Speed;
			//
			//float waveValueA = sin(time + v.vertex.x * _Frequency) * _Amplitude;  // 사인파를 이용한 파형
			//			
			//v.vertex.xyz = float3(v.vertex.x, v.vertex.y + waveValueA, v.vertex.z);  // 버텍스 쪽에 직접 더해서 만듭니다.
			//
			//v.normal = normalize(float3(v.normal.x + waveValueA, v.normal.y, v.normal.z));

			
			UNITY_INITIALIZE_OUTPUT(Input, o);
			
			float time = _Time * _Speed;
			
			float waveValueA = sin(time + v.vertex.x * _Frequency) * _Amplitude;
			
			float waveValueB = cos(time + v.vertex.y * _Frequency) * _Amplitude;

			float waveValueC = sin(time*1.5 + (v.vertex.z + v.vertex.y) * _Frequency) * _Amplitude;

			float waveValueD = cos(time*1.5 + (v.vertex.z + v.vertex.y) * _Frequency) * _Amplitude;
			
			float waveValueE = sin(time*0.5 + (v.vertex.z + v.vertex.x) * _Frequency*0.3) * _Amplitude;

			float waveValueF = cos(time*0.5 + (v.vertex.z + v.vertex.x) * _Frequency*0.3) * _Amplitude;


			float3 VMove = float3(v.vertex.x + waveValueA, v.vertex.y + waveValueB, v.vertex.z);

			VMove = VMove + float3(v.vertex.x + waveValueC, v.vertex.y + waveValueD, v.vertex.z);

			VMove = VMove + float3(v.vertex.x + waveValueF, v.vertex.y + waveValueE, v.vertex.z);

			v.vertex.xyz = VMove;
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
