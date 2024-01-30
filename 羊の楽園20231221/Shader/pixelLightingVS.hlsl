#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //���񂨖񑩂̒��_�ϊ�
	wvp = mul(World, View);					//���[���h�ϊ��s��~�r���[�ϊ��s��
	wvp = mul(wvp, Projection);				//���~�v���W�F�N�V�����ϊ�
	Out.Position = mul(In.Position, wvp);	//���͂��ꂽ���_����ϊ����ďo�͐�֑������


	//���_�@�������[���h�s��ŉ�]������(���_�Ɠ�����]��������)
	float4 worldNormal, normal;
	normal = float4(In.Normal.xyz, 0.0); //�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
	worldNormal = mul(normal, World);	 //�@�������[���h�s��ŉ�]����
	worldNormal = normalize(worldNormal);//��]��̖@���𐳋K������
	Out.Normal = worldNormal;			 //��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��
	
	Out.Diffuse = In.Diffuse; //���͒��_�̕������̂܂܏o��
	Out.TexCoord = In.TexCoord; //���_�̃e�N�X�`�����W���o��


	//Out.Diffuse = In.Diffuse;

	//�o�͐�փ[����������
	Out.WorldPosition = float4(0.0, 0.0, 0.0, 0.0);

	Out.WorldPosition = mul(In.Position, World);
	//Out.Normal = In.Normal;]

}