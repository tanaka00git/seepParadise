#pragma once
#include "..\App\component.h"

class Sprite : public Component
{
protected:
	ID3D11Buffer*m_VertexBuffer;
	ID3D11ShaderResourceView* m_Texture{};
	D3DXCOLOR	m_Color	   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Position = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Scale    = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

public:
	void Init(float x, float y, float Width, float Height, const char* TextureName);
	void Uninit();
	void Update();
	void Draw();

	void SetColor(D3DXCOLOR Color) { m_Color = Color; }				  //色変え用
	void SetPosition(D3DXVECTOR3 position) { m_Position = position; } //ポジション変更用
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }			  //サイズ変更用
};