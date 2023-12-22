#pragma once
#include "..\GameObject\gameObject.h"

class ResultCoin : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Model* m_Model;
	
	//メンバ変数
	D3DXVECTOR3 m_Velocity{};	//速度
	bool m_DropFirst = true;	//ジャンプ初回
	
public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetDrop(){ m_DropFirst = true;};
};