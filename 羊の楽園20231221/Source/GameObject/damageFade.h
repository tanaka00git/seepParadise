#pragma once
#include "..\GameObject\gameObject.h"

class DamageFade : public GameObject
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Sprite* m_Sprite;//ëOï˚êÈåæ

	D3DXCOLOR m_Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float m_Alpha = 0.0f;
	bool m_Fade = true;
public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetColorData(D3DXCOLOR color) { m_Color = color; }

};