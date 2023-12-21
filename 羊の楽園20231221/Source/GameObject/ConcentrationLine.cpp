#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\concentrationLine.h"
#include "..\App\sprite.h"
#include "..\App\manager.h"
#include "..\Scene\scene.h"
#include "..\GameObject\player.h"

Sprite*ConcentrationLine::m_Sprite1{};
Sprite*ConcentrationLine::m_Sprite2{};
Sprite*ConcentrationLine::m_Sprite3{};
Sprite*ConcentrationLine::m_Sprite4{};

void ConcentrationLine::Load()
{
	m_Sprite1 = new Sprite();
	m_Sprite1->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\ConcentrationLine1.png");
	m_Sprite2 = new Sprite();
	m_Sprite2->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\ConcentrationLine2.png");
	m_Sprite3 = new Sprite();
	m_Sprite3->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\ConcentrationLine3.png");
	m_Sprite4 = new Sprite();
	m_Sprite4->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\ConcentrationLine4.png");
}

void ConcentrationLine::Unload()
{
}

void ConcentrationLine::Init()
{
	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Alpha1 = 0.0f;
	m_Alpha2 = 0.0f;
	m_Alpha3 = 0.0f;
	m_Alpha3 = 0.0f;
}

void ConcentrationLine::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void ConcentrationLine::Update()
{
	GameObject::Update();

	//ダッシュ演出
	m_DeleteTime ++;

	if (m_DeleteTime % 5 == 0) 
	{
		if (a == 1) 
		{
			m_Alpha1 = 0.7f;
			m_Alpha2 = 0.5f;
			m_Alpha3 = 0.2f;
			m_Alpha4 = 0.2f;
			a = 2;
		}
		else if (a == 2) 
		{
			m_Alpha1 = 0.2f;
			m_Alpha2 = 0.7f;
			m_Alpha3 = 0.5f;
			m_Alpha4 = 0.2f;
			a = 3;
		}
		else if (a == 3) 
		{
			m_Alpha1 = 0.2f;
			m_Alpha2 = 0.2f;
			m_Alpha3 = 0.7f;
			m_Alpha4 = 0.5f;
			a = 4;
		}
		else if (a == 4)
		{
			m_Alpha1 = 0.5f;
			m_Alpha2 = 0.2f;
			m_Alpha3 = 0.2f;
			m_Alpha4 = 0.7f;
			a = 1;
		}
	}

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	PLAYER_STATE playerState = player->GetState();
	if (playerState != PLAYER_STATE::DASH) { m_AlphaAll -= 0.1f; }
	if (m_AlphaAll <= 0) { SetDestroy(); }

	m_Sprite1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha1 * m_AlphaAll));
	m_Sprite2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha2 * m_AlphaAll));
	m_Sprite3->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha3 * m_AlphaAll));
	m_Sprite4->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha4 * m_AlphaAll));
}

void ConcentrationLine::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// 使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	m_Sprite1->Draw();
	m_Sprite2->Draw();
	m_Sprite3->Draw();
	m_Sprite4->Draw();

	//基底クラスメソッドの呼び出し
	GameObject::Draw();
}

