#pragma once
#include "..\GameObject\gameObject.h"

class ResultSheep : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	
	static class Model* m_Model;

	//メンバ変数
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);//速度

	int   m_Speed = 7;
	bool  m_Stop = false;
	float m_AnimeTime = 0.0f;
	bool  m_AnimePause = true;

	float m_OrientationTime = 0.0f;	 //向き変更時間
	float m_NextRotTime = 0.0f;		 //次に向き変更するまで
	int   m_NextRot = 1;			 //次の向き

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

};