#pragma once
#include "..\GameObject\gameObject.h"

class HpBarS : public Component
{
private:
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	ID3D11Buffer* m_VertexBuffer = nullptr;
	static ID3D11ShaderResourceView* m_Texture;
	static ID3D11ShaderResourceView* m_Texture2;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	float m_FullLife	   = 1.0f;
	float m_CurrentLife    = 1.0f;
	float m_OldConversion  = 0.0f;
	float m_OldConversion2 = 0.0f;

	//プレイヤーのチャージ表示
	bool  m_ChargeOn		   = false;
	bool  m_DashOn		   = false;
	float m_FullCharge	   = 0.0f;
	float m_CurrentCharge  = 0.0f;

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPosition(D3DXVECTOR3 position) { m_Position = position; }
	void SetLifeDateFC(int full, int current) { m_FullLife = static_cast<float>(full); m_CurrentLife = static_cast<float>(current); }
	void SetCharge(int currentCharge, int fullCharge, bool dash) { m_CurrentCharge = static_cast<float>(currentCharge); m_FullCharge = static_cast<float>(fullCharge); m_DashOn = dash; m_ChargeOn = true; }
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }//色変え用

};