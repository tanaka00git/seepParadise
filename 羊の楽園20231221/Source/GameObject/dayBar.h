#pragma once
#include "..\GameObject\gameObject.h"

class DayBar : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Sprite* m_Sprite;
	static class Sprite* m_Sprite2;
	float m_Alpha = 0.0f;

	float m_FullTime	  = 40.0f;
	float m_CurrentTime	  = 0.0f;
	float m_OldConversion = 0.0f;

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetTimeFC(int f, int c) { m_FullTime = static_cast<float>(f); m_CurrentTime = static_cast<float>(c); }

};