#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\infoLogTutorial.h"
#include "..\App\sprite.h"

void InfoLogTutorial::Init()
{

	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Sprite_Tutorial1 = AddComponent<Sprite>();
	m_Sprite_Tutorial1->Init(0.0f, 0.0f, 600, 60, "asset\\texture\\tutorialLog1.png");
	m_Sprite_Tutorial2 = AddComponent<Sprite>();
	m_Sprite_Tutorial2->Init(0.0f, 0.0f, 600, 60, "asset\\texture\\tutorialLog2.png");
	m_Sprite_Tutorial3 = AddComponent<Sprite>();
	m_Sprite_Tutorial3->Init(0.0f, 0.0f, 600, 60, "asset\\texture\\tutorialLog3.png");
	m_Sprite_Tutorial4 = AddComponent<Sprite>();
	m_Sprite_Tutorial4->Init(0.0f, 0.0f, 600, 60, "asset\\texture\\tutorialLog4.png");
	m_Alpha4 = 0.0f;

	m_Position.x = 180;
	m_Position.y = 450;
	m_Sprite_Tutorial1->SetPosition(m_Position);
	m_Sprite_Tutorial2->SetPosition(m_Position);
	m_Sprite_Tutorial3->SetPosition(m_Position);
	m_Sprite_Tutorial4->SetPosition(m_Position);
}

void InfoLogTutorial::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}


void InfoLogTutorial::Update()
{
	GameObject::Update();


	if (m_Log == 1)
	{
		m_Alpha = 1.0f;
		m_Alpha2 = 0.0f;
		m_Alpha3 = 0.0f;
		m_Sprite_Tutorial4->SetPosition(D3DXVECTOR3(100, 450, 0));
		m_ControlTime ++;
		if (m_ControlTime >= 270) { m_Alpha4 += 0.3f; }
		if (m_ControlTime >= 360) { m_Log ++; m_ControlTime = 0; }
	}
	else if (m_Log == 2)
	{
		if (!m_LogCharge)
		{
			m_Alpha = 0.0f;
			m_Alpha2 = 1.0f;
			m_Alpha3 = 0.0f;
			m_Alpha4 = 0.0f;
			m_Sprite_Tutorial4->SetPosition(D3DXVECTOR3(140, 450,0));
		}
		else
		{
			m_Alpha4 += 0.3f;
			m_ControlTime ++;
			if (m_ControlTime >= 270) { m_Log ++; m_ControlTime = 0; }
		}

	}
	else if (m_Log == 3)
	{
		if (!m_LogAttack)
		{
			m_Alpha = 0.0f;
			m_Alpha2 = 0.0f;
			m_Alpha3 = 1.0f;
			m_Alpha4 = 0.0f;
			m_Sprite_Tutorial4->SetPosition(D3DXVECTOR3(180, 450,0));

		}
		else
		{
			m_Alpha4 += 0.3f;
			m_ControlTime ++;
			if (m_ControlTime >= 270) { m_Log ++; m_ControlTime = 0; }
		}
	}
	else if (m_Log == 4) {
		SetDestroy();
	}

	//上限
	if (m_Alpha4 > 1.0f) { m_Alpha4 = 1.0f; }

	m_Sprite_Tutorial1->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
	m_Sprite_Tutorial2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha2));
	m_Sprite_Tutorial3->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha3));
	m_Sprite_Tutorial4->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha4));
}


void InfoLogTutorial::Draw()
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