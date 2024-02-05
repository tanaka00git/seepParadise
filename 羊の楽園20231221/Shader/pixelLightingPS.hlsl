#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);
	float light = -dot(Light.Direction.xyz, normal.xyz);

	light = saturate(light * 2);

	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord); //�e�N�X�`���̐F���擾

	outDiffuse.rgb *= light;
	outDiffuse.a *= In.Diffuse.a;

	//�t�H��
	//�J��������s�N�Z���֌������x�N�g��
	//�����x�N�g��
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev); //���K������

	//�������C�e�B���O
	float rim = dot(eyev.xyz, normal.xyz) + 1.0f; //�����Ɩ@���̓��ς𖾂邳�ɕϊ�����
	rim = pow(rim, 6) * 2.0f; //�X�y�L�����Ɠ����悤�ȏ�����K���ɍs���B
	rim = saturate(rim); //rim���T�`�����[�g����

	outDiffuse.r += rim; //�ʏ�̐F�։��Z����B
	outDiffuse.a = In.Diffuse.a;
}
