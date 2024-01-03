#pragma once
#include "..\GameObject\gameObject.h"

class MouseIcon : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Sprite* m_Sprite{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};