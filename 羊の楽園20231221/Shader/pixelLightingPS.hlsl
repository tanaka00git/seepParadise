#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);
	float light = -dot(Light.Direction.xyz, normal.xyz);

	light = saturate(light * 2);

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord); //テクスチャの色を取得

	outDiffuse.rgb *= light;
	outDiffuse.a *= In.Diffuse.a;

	//フォン
	//カメラからピクセルへ向かうベクトル
	//視線ベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //正規化する

	//リムライティング
	float rim = dot(eyev.xyz, normal.xyz) + 1.0f; //視線と法線の内積を明るさに変換する
	rim = pow(rim, 6) * 2.0f; //スペキュラと同じような処理を適当に行う。
	rim = saturate(rim); //rimをサチュレートする

	outDiffuse.r += rim; //通常の色へ加算する。
	outDiffuse.a = In.Diffuse.a;
}
