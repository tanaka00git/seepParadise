#pragma once
#include "..\App\component.h"

class GoalScope : public Component
{
private:
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	ID3D11Buffer* m_VertexBuffer{};
	static ID3D11ShaderResourceView* m_Texture;


	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

public:

	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR color) { m_Color = color; }//�F�ς��p
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }//�F�ς��p
	void SetPosition(D3DXVECTOR3 position) { m_Position = position; }
	D3DXVECTOR3 GetPosition() { return m_Position; }
};