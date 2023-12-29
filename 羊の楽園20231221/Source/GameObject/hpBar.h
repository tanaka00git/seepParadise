#pragma once
#include "..\GameObject\gameObject.h"

class HpBar : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Sprite* m_Sprite_BlackLife{};
	class Sprite* m_Sprite_RedLife{};
	class Sprite* m_Sprite_GreenLife{};
	class Sprite* m_Sprite_BlackCharge{};
	class Sprite* m_Sprite_WhiteCharge{};

	float m_Alpha = 0.0f;
	float m_Alpha2 = 0.0f;

	float m_FullLife = 0.0f;
	float m_CurrentLife = 0.0f;
	float m_OldConversion = 0.0f;
	float m_OldConversion2 = 0.0f;

	float m_FullCharge = 0.0f;
	float m_CurrentCharge = 0.0f;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void LifeBarMove();
	void ChargeBarMove();
};