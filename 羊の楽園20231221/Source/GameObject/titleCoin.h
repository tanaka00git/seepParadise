#pragma once
#include "..\GameObject\gameObject.h"

class TitleCoin : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Sprite* m_Sprite{};
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);	//‘¬“x
	float		m_Alpha	   = 1.0f;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};