#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\App\audio.h"
#include "..\Scene\scene.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\hpBarS.h"

Audio*BreakObject::m_SE_Kick{};

#define GRAVITY 0.015f
#define GIVE_ATTACK_STOP 20

void BreakObject::Load()
{
	m_SE_Kick = new Audio();
	m_SE_Kick->Load("asset\\audio\\小キックb.wav");
}

void BreakObject::Unload()
{
}

void BreakObject::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_HpBarS = AddComponent<HpBarS>();
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
	m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_Scale.y = 0.01f;
}

void BreakObject::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BreakObject::Update()
{
	GameObject::Update();

	m_DaathTime--;
	if (m_DaathTime <= 0) { m_BreakObjectState = BREAKOBJECT_STATE::ORTDEATH; }

	switch (m_BreakObjectState)
	{
	case BREAKOBJECT_STATE::NORMAL:
		UpdateMove();
		break;
	case BREAKOBJECT_STATE::DEATH:
		UpdateDead();
		break;
	case BREAKOBJECT_STATE::ORTDEATH:
		UpdateDelete();
		break;
	}
}

void BreakObject::UpdateMove()
{
	Scene* scene = Manager::GetScene();

	//ぬるぬる出現
	m_Scale.x += 0.05f; m_Scale.y += 0.05f; m_Scale.z += 0.05f;
	if (m_Scale.x >= m_OriginalScale.x) { m_Scale.x = m_OriginalScale.x; }
	if (m_Scale.y >= m_OriginalScale.y) { m_Scale.y = m_OriginalScale.y; }
	if (m_Scale.z >= m_OriginalScale.z) { m_Scale.z = m_OriginalScale.z; }

	//HPバー表示
	LifeBarMove();

	//重力
	m_Velocity.y -= GRAVITY;

	//移動
	m_Position += m_Velocity;	

	//障害物との衝突判定
	float groundHeight = 0.0f;

	//接地
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	//ライフがなくなったら
	if (m_Life <= 0)
	{
		m_Life = 0;
		m_BreakObjectState = BREAKOBJECT_STATE::DEATH;
	}
}

void BreakObject::UpdateDelete()
{
	//ぬるぬる消滅
	m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Scale.x -= 0.05f; m_Scale.y -= 0.05f; m_Scale.z -= 0.05f;
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void BreakObject::UpdateDead()
{
	m_Position.y += 0.5f;
	m_Scale.x -= 0.05f; m_Scale.y -= 0.05f; m_Scale.z -= 0.05f;
	if (m_Scale.y < 0.0f) { SetDestroy(); }
}

void BreakObject::SetDamageMove()
{
	m_Life --;
	m_SE_Kick->Play(1.0f);
	m_Velocity.y = 0.1f;
}

void BreakObject::LifeBarMove()
{
	//スケール設定
	if (m_FullLife != m_Life) { m_HpBarS->SetScale(D3DXVECTOR3(1.2f, 0.8f, 0.8f)); }
	else { m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); }

	//HPバーの位置変更
	D3DXVECTOR3 HpBarPosition = m_Position;
	HpBarPosition.y += 1.5f;
	m_HpBarS->SetPosition(HpBarPosition);

	//情報送信
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
}
