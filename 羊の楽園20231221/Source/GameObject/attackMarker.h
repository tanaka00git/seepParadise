#pragma once
#include "..\App\component.h"

class AttackMarker : public Component
{
private:
	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_ObjectScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	ID3D11Buffer* m_VertexBuffer{};
	static ID3D11ShaderResourceView* m_Texture;


	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

public:
	void SetPosition(D3DXVECTOR3 position) { m_Position = position; }

	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR Color) { m_Color = Color; }//色変え用
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; }//スケール用
	void SetRotation(D3DXVECTOR3 rotation) { m_Rotation = rotation;}//向き変更用
};