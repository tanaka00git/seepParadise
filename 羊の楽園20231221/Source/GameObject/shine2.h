#pragma once
#include "..\GameObject\gameObject.h"

class Shine2 : public GameObject
{
private:

	ID3D11Buffer* m_VertexBuffer = nullptr;
	static ID3D11ShaderResourceView* m_Texture;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//メンバ変数
	int m_Count = 0;
	int m_CountFrameDelay = 0;

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
