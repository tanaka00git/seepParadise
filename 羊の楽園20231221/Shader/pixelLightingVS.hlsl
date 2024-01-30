#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //毎回お約束の頂点変換
	wvp = mul(World, View);					//ワールド変換行列×ビュー変換行列
	wvp = mul(wvp, Projection);				//↑×プロジェクション変換
	Out.Position = mul(In.Position, wvp);	//入力された頂点情報を変換して出力先へ代入する


	//頂点法線をワールド行列で回転させる(頂点と同じ回転をさせる)
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0); //法線ベクトルのwを0としてコピー（平行移動しないため)
	worldNormal = mul(normal, World);	 //法線をワールド行列で回転する
	worldNormal = normalize(worldNormal);//回転後の法線を正規化する
	Out.Normal = worldNormal;			 //回転後の法線出力 In.Normalでなく回転後の法線を出力
	
	Out.Diffuse = In.Diffuse; //αは頂点の物をそのまま出力
	Out.TexCoord = In.TexCoord; //頂点のテクスチャ座標を出力


	//Out.Diffuse = In.Diffuse;

	//出力先へゼロを代入する
	Out.WorldPosition = float4(0.0, 0.0, 0.0, 0.0);

	Out.WorldPosition = mul(In.Position, World);
	//Out.Normal = In.Normal;]

}