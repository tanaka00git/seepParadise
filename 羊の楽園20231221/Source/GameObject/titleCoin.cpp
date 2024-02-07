#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\titleCoin.h"
#include "..\App\sprite.h"
#include "..\App\input.h"

#define SCALE 30.0f
#define GRAVITY 0.15f

void TitleCoin::Init()
{
	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	// マウスの座標を取得
	POINT mousePos = Input::GetMousePosition();
	m_Position = D3DXVECTOR3(static_cast<float>(mousePos.x) - SCALE / 2, static_cast<float>(mousePos.y) - SCALE / 2, 0.0f - SCALE / 2);

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(m_Position.x, m_Position.y, SCALE, SCALE, "asset\\texture\\coin.png");
	m_Sprite->SetPosition(m_Position);

	//飛ぶ
	m_Velocity.x = frand() * 5.0f - 2.5f;
	m_Velocity.y = -3;
}

void TitleCoin::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}


void TitleCoin::Update()
{
	GameObject::Update();

	//重力
	m_Velocity.y += GRAVITY;

	if (m_Velocity.y >= 0)
	{
		m_Alpha -= 0.01f;
	}
	if (m_Alpha <= 0.0f)
	{
		SetDestroy();
	}
	m_Position += m_Velocity;
	m_Sprite->SetPosition(m_Position);
	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}


void TitleCoin::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// 使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//基底クラスメソッドの呼び出し
	GameObject::Draw();
}