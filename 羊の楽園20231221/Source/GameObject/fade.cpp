#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\fade.h"
#include "..\App\sprite.h"

void Fade::Init()
{

	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\fade.png");
}

void Fade::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}


void Fade::Update()
{
	GameObject::Update();


	//フェードイン処理
	if (!m_FadeOut)
	{
		m_Alpha -= 0.05f;
		if (m_Alpha < 0.0f)
			m_Alpha = 0.0f;
	}
	else{
		m_Alpha += 0.02f;
		if (m_Alpha > 1.0f) {
			m_Alpha = 1.0f;
			m_FadeFinish = true;
		}
	}
	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}

void Fade::Draw()
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