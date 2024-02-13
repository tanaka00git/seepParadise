#pragma once
#include "..\GameObject\gameObject.h"

class ResultSheep : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	
	static class Model* m_Model;

	//�����o�ϐ�
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);//���x

	int   m_Speed = 7;
	bool  m_Stop = false;
	float m_AnimeTime = 0.0f;
	bool  m_AnimePause = true;

	float m_OrientationTime = 0.0f;	 //�����ύX����
	float m_NextRotTime = 0.0f;		 //���Ɍ����ύX����܂�
	int   m_NextRot = 1;			 //���̌���

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

};