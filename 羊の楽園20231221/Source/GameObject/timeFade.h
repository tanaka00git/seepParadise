#pragma once
#include "..\GameObject\gameObject.h"

class TimeFade : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Sprite* m_Sprite;
	static class Sprite* m_Sprite2;
	float m_Alpha  = 0.0f;
	float m_Alpha2 = 0.0f;

	bool  m_FadeOut = false;
	bool  m_FadeOut2 = false;
	bool  m_FadeFinish = false;
public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void FadeOut() { m_FadeOut = !m_FadeOut; }
	void FadeOut_true() { m_FadeOut = true; }
	void FadeOut_false() { m_FadeOut = false; }
	bool GetFadeFinish() { return m_FadeFinish; }

	bool GetTimeZone() { return m_FadeOut2; }

};