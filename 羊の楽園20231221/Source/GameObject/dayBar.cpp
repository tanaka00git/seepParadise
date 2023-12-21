#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\dayBar.h"
#include "..\App\sprite.h"
#include "..\GameObject\player.h"

Sprite*DayBar::m_Sprite{};
Sprite*DayBar::m_Sprite2{};

void DayBar::Load()
{
	m_Sprite = new Sprite();
	m_Sprite->Init(845.0f, 70.0f, 80, 7, "asset\\texture\\fadeOver.png");
	m_Sprite->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));//黒バー

	m_Sprite2 = new Sprite();
	m_Sprite2->Init(845.0f, 70.0f, 80, 7, "asset\\texture\\fadeOver.png");
	m_Sprite2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//白バー

}

void DayBar::Unload()
{
}

void DayBar::Init()
{
	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void DayBar::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}


void DayBar::Update()
{
	GameObject::Update();
	
	float Conversion = (m_CurrentTime / m_FullTime) * 80;

	float distance = Conversion - m_OldConversion;
	float speed = 0.1f; // 移動速度を調整する値
	float velocity = distance * speed;
	float newConversion = m_OldConversion + velocity;
	Conversion = newConversion;
	m_Sprite2->SetScale(D3DXVECTOR3(Conversion, 7, 0));
	m_OldConversion = Conversion;
}


void DayBar::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// 使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	m_Sprite->Draw();
	m_Sprite2->Draw();

	//基底クラスメソッドの呼び出し
	GameObject::Draw();
}