#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\App\audio.h"
#include "..\Scene\scene.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\box.h"

Audio*BreakObject::m_SE_Kick{};

// 定数
const D3DXCOLOR INITIAL_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
const bool INITIAL_TEXTURE_ENABLE = true;
const D3DXVECTOR3 INITIAL_VELOCITY = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 INITIAL_ORIGINAL_SCALE = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
const bool INITIAL_DELETE_INIT = false;
const int INITIAL_DEATH_TIME = 2000;
const int INITIAL_DAMAGE_FLASH_TIME = 0;
const int INITIAL_LIFE = 7;
const D3DXVECTOR3 INITIAL_BAR_SCALE = D3DXVECTOR3(1.2f, 0.8f, 0.8f);
const float INITIAL_HP_BAR_POS_Y = 1.5f;

const float GRAVITY = 0.015f;
const int DAMAGE_FLASH_TIME = 6;
const float INITIAL_SCALE_Y = 0.01f;

void BreakObject::Load()
{
	m_SE_Kick = new Audio();
	m_SE_Kick->Load("asset\\audio\\小キックb.wav");
}

void BreakObject::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
	
	//初期化
	m_Color = INITIAL_COLOR;
	m_TextureEnable = INITIAL_TEXTURE_ENABLE;
	m_Velocity = INITIAL_VELOCITY;
	m_OriginalScale = INITIAL_ORIGINAL_SCALE;
	m_DeleteInit = INITIAL_DELETE_INIT;
	m_DaathTime = INITIAL_DEATH_TIME;
	m_DamageFlashTime = INITIAL_DAMAGE_FLASH_TIME;
	m_FullLife = INITIAL_LIFE;
	m_Life = INITIAL_LIFE;
	m_BarScale = INITIAL_BAR_SCALE;
	m_Scale.y = INITIAL_SCALE_Y;

	//アタッチ
	m_HpBarS = AddComponent<HpBarS>();
	m_HpBarS->SetLifeDateFC(INITIAL_LIFE, INITIAL_LIFE);
	m_HpBarS->SetScale(m_BarScale);
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

	//当たり判定
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//ダメージフラッシュ
	DamageFlash();
}

void BreakObject::UpdateMove()
{
	Scene* scene = Manager::GetScene();

	//ぬるぬる出現
	SmoothAppearance(true);

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
	m_HpBarS->SetDraw(false);
	SmoothAppearance(false);
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void BreakObject::UpdateDead()
{
	m_HpBarS->SetDraw(false);
	m_Position.y += 0.5f;
	SmoothAppearance(false);
	if (m_Scale.y < 0.0f) { SetDestroy(); }
}

void BreakObject::SetDamageMove()
{
	m_Life --;
	m_SE_Kick->Play(1.0f);
	m_Velocity.y = 0.1f;
	m_DamageFlashTime = DAMAGE_FLASH_TIME;
}

void BreakObject::DamageFlash()
{
	if (m_DamageFlashTime > 0)
	{
		m_DamageFlashTime--;
		m_TextureEnable = false;
	}
	else {
		m_TextureEnable = true;
	}
}

void BreakObject::LifeBarMove()
{
	//スケール設定
	//HPが最大HPと一緒じゃなければライフバー表示
	if (m_FullLife != m_Life) { m_HpBarS->SetDraw(true); }

	//HPバーの位置変更
	D3DXVECTOR3 HpBarPosition = m_Position;
	HpBarPosition.y += m_HpBarPosY;
	m_HpBarS->SetPosition(HpBarPosition);

	//情報送信
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
}

void BreakObject::Collision(float& groundHeight)
{
	Scene* scene = Manager::GetScene();

	// 円柱
	auto cylinders = scene->GetGameObjects<Cylinder>();    // 円柱のリストを取得
	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		scale.x *= 1.2f, scale.z *= 1.2f;					//半径の調整
		D3DXVECTOR3 direction = m_Position - position;		//円柱中心からキャラまでのベクトル
		D3DXVECTOR3 up = cylinder->GetUp();					//円柱の上方向ベクトル
		float obbY = D3DXVec3Dot(&direction, &up);			//円柱の上方向への軸分離

		//影の高さを調整するため用
		direction -= up * obbY;								//y軸の接触を無視するために、directionからupの成分を引く
		float length = D3DXVec3Length(&direction);			//新しいlengthを計算する（キャラと円柱中心の距離）

		//当たり判定
		if (length < scale.x && fabs(obbY) < scale.y)
		{
			D3DXVECTOR3 normalizedDirection = direction / length;
			float penetrationY = scale.y - static_cast<float>(fabs(obbY));
			float penetrationX = scale.x - length;

			// 円柱内に入った場合の処理
			if (penetrationX < penetrationY)
			{
				// 入った方向に押し出し処理
				m_Position += penetrationX * normalizedDirection;
			}
			else
			{
				m_Position += (m_Position.y < obbY) ? (-penetrationY * up) : penetrationY * up;
				m_Velocity.y = (m_Position.y < obbY) ? -m_Velocity.y : 0.0f; // 上に乗ったら垂直速度を0にする、下から触れたら反転する
			}
		}
	}

	//直方体
	auto boxs = scene->GetGameObjects<Box>();	//リストを取得
	for (Box* box : boxs)						//範囲forループ
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();
		scale.y *= 1.8f;								//高さ調整
		D3DXVECTOR3 right = box->GetRight();			//x軸分離
		D3DXVECTOR3 forward = box->GetForward();		//z軸分離
		D3DXVECTOR3 up = box->GetUp();					//y軸分離
		D3DXVECTOR3 direction = m_Position - position;	//直方体からキャラまでの方向ベクトル
		float obbX = D3DXVec3Dot(&direction, &right);
		float obbZ = D3DXVec3Dot(&direction, &forward);
		float obbY = D3DXVec3Dot(&direction, &up);
		//OBB
		if (fabs(obbX) < scale.x && fabs(obbZ) < scale.z && fabs(obbY) < scale.y)
		{
			//どれだけ押し出すかを調べる
			D3DXVECTOR3 penetration = D3DXVECTOR3(static_cast<float>(scale.x - fabs(obbX)), static_cast<float>(scale.y - fabs(obbY)), static_cast<float>(scale.z - fabs(obbZ)));

			//左右から触れる
			if (penetration.x < penetration.z && penetration.x < penetration.y)
			{
				m_Position += (obbX > 0) ? (penetration.x * right) : (-penetration.x * right);
			}
			//前後から触れる
			else if (penetration.z < penetration.y)
			{
				m_Position += (obbZ > 0) ? (penetration.z * forward) : (-penetration.z * forward);
			}
			//上下から触れる
			else
			{
				m_Position += (m_Position.y < obbY) ? (-penetration.y * up) : penetration.y * up;
				m_Velocity.y = (m_Position.y < obbY) ? -m_Velocity.y : 0.0f; // 上に乗ったら垂直速度を0にする、下から触れたら反転する
			}
		}
	}

	if (m_Position.y < 0.0f && m_Velocity.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Velocity.y = 0.0f;
	}
}

void BreakObject::SmoothAppearance(bool growing)
{
	if (growing)
	{
		m_Scale.x += m_OriginalScale.x / 20;
		m_Scale.y += m_OriginalScale.y / 20;
		m_Scale.z += m_OriginalScale.y / 20;
		if (m_Scale.x >= m_OriginalScale.x) { m_Scale.x = m_OriginalScale.x; }
		if (m_Scale.y >= m_OriginalScale.y) { m_Scale.y = m_OriginalScale.y; }
		if (m_Scale.z >= m_OriginalScale.z) { m_Scale.z = m_OriginalScale.z; }
	}
	else
	{
		m_Scale.x -= m_OriginalScale.y / 20;
		m_Scale.y -= m_OriginalScale.y / 20;
		m_Scale.z -= m_OriginalScale.y / 20;
		if (m_Scale.x <= 0.0f) { m_Scale.x = 0.0f; }
		if (m_Scale.y <= 0.0f) { m_Scale.y = 0.0f; }
		if (m_Scale.z <= 0.0f) { m_Scale.z = 0.0f; }
	}
}
