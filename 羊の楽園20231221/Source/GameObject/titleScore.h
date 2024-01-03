#pragma once
#include "..\GameObject\gameObject.h"

class TitleScore : public GameObject
{
private:

	ID3D11Buffer*m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture{};
	ID3D11ShaderResourceView* m_Texture2{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_CountCoin{};
	float m_Alpha = 0.0f;
	bool m_AlphaMove = false;
	int m_PlColor;
	int m_PlClown;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddCountCoin(int CountCoin) { m_CountCoin += CountCoin; }
	void SetCountCoin(int CountCoin) { m_CountCoin = CountCoin; }
	void SetColorClown(int color, int clown) { m_PlColor = color;  m_PlClown = clown; }
	void SetAlpha(bool alpha) { m_AlphaMove = alpha;};

};