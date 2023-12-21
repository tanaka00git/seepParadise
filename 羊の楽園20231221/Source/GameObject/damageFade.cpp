#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\damageFade.h"
#include "..\App\sprite.h"

Sprite*DamageFade::m_Sprite{};

void DamageFade::Load()
{
	m_Sprite = new Sprite();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\fadeOver.png");
}

void DamageFade::Unload()
{
	m_Sprite->Uninit();
}

void DamageFade::Init()
{
	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void DamageFade::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void DamageFade::Update()
{
	GameObject::Update();


	if (m_Fade) {
		m_Alpha = 0.3f;
		m_Fade = false;
	}

	m_Alpha -= 0.01f;
	if (m_Alpha <= 0.0f){SetDestroy();}

	m_Color.a = m_Alpha;
	m_Sprite->SetColor(m_Color);
}

void DamageFade::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// 使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	m_Sprite->Draw();

	//基底クラスメソッドの呼び出し
	GameObject::Draw();
}