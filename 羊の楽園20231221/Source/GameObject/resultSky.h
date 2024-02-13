#pragma once
#include "..\GameObject\gameObject.h"

class ResultSky : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXCOLOR	m_Color			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//îzêF
	float		m_ColorValue	= 0.0f;										//êFÇÃã≠Ç≥Å™
	float		m_CountLight	= 0.0f;									//ñæÇÈÇ≥

	static class Model* m_Model;

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetCountLight(int countLight) { m_CountLight = static_cast<float>(countLight); }
};