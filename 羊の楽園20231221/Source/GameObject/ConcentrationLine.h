#pragma once
#include "..\GameObject\gameObject.h"

class ConcentrationLine : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Sprite* m_Sprite1;
	static class Sprite* m_Sprite2;
	static class Sprite* m_Sprite3;
	static class Sprite* m_Sprite4;

	int a = 1;
	float m_Alpha1 = 0.0f;
	float m_Alpha2 = 0.0f;
	float m_Alpha3 = 0.0f;
	float m_Alpha4 = 0.0f;
	float m_AlphaAll = 1.0f;
	int	  m_DeleteTime = 0;
	bool  m_LogEnd = false;
public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

};