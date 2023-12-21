#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\hpBar.h"
#include "..\App\sprite.h"
#include "..\GameObject\player.h"


void HpBar::Init()
{
	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Sprite_BlackLife = AddComponent<Sprite>();
	m_Sprite_BlackLife->Init(10.0f, 10.0f, 180, 10, "asset\\texture\\fadeOver.png");
	m_Sprite_BlackLife->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));//黒バー
	m_Sprite_RedLife = AddComponent<Sprite>();
	m_Sprite_RedLife->Init(10.0f, 10.0f, 180, 10, "asset\\texture\\fadeOver.png");
	m_Sprite_RedLife->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));//赤バー
	m_Sprite_GreenLife = AddComponent<Sprite>();
	m_Sprite_GreenLife->Init(10.0f, 10.0f, 180, 10, "asset\\texture\\fadeOver.png");
	m_Sprite_GreenLife->SetColor(D3DXCOLOR(0.4f, 0.8f, 0.4f, 1.0f));//緑バー
	m_Sprite_BlackCharge = AddComponent<Sprite>();
	m_Sprite_BlackCharge->Init(10.0f, 20.0f, 180, 5, "asset\\texture\\fadeOver.png");
	m_Sprite_BlackCharge->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));//黒バーチャージ
	m_Sprite_WhiteCharge = AddComponent<Sprite>();
	m_Sprite_WhiteCharge->Init(10.0f, 20.0f, 180, 5, "asset\\texture\\fadeOver.png");
	m_Sprite_WhiteCharge->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//白バーチャージ
}

void HpBar::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}


void HpBar::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	m_FullLife = static_cast<float>(player->GetFullLife());
	m_CurrentLife = static_cast<float>(player->GetLife());
	
	float Conversion = (m_CurrentLife / m_FullLife) * 180;
	float Conversion2 = Conversion;

	//緑バー
	float distance = Conversion - m_OldConversion;
	float speed = 0.2f; // 移動速度を調整する値
	float velocity = distance * speed;
	float newConversion = m_OldConversion + velocity;
	Conversion = newConversion;
	m_Sprite_GreenLife->SetScale(D3DXVECTOR3(Conversion, 10, 0));
	m_OldConversion = Conversion;

	//赤バー
	distance = Conversion2 - m_OldConversion2;
	speed = 0.1f; // 移動速度を調整する値
	velocity = distance * speed;
	newConversion = m_OldConversion2 + velocity;
	Conversion2 = newConversion;
	m_Sprite_RedLife->SetScale(D3DXVECTOR3(Conversion2, 10, 0));
	m_OldConversion2 = Conversion2;

	//チャージ
	m_CurrentCharge = static_cast<float>(player->GetCharge());
	m_FullCharge = static_cast<float>(player->GetFullCharge());
	
	//チャージ上限時の色変更
	if (m_CurrentCharge >= m_FullCharge)
	{
		m_CurrentCharge = m_FullCharge;
		m_Sprite_WhiteCharge->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
	}
	else { m_Sprite_WhiteCharge->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }//白バー

	float Conversion3 = (m_CurrentCharge / m_FullCharge) * 180;
	m_Sprite_WhiteCharge->SetScale(D3DXVECTOR3(Conversion3, 5, 0));

}


void HpBar::Draw()
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