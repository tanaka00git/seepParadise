#pragma once
#include "..\GameObject\gameObject.h"

class ResultCoin : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};//‘¬“x

	static class Model* m_Model;

	int m_DaathTime = 1000;
	bool m_DropFirst = true;
	bool m_DropJump = false;
	
public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetDrop() {
		m_DropFirst = true;
		m_DropJump = true;
	};
};