#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\GameObject\characterObject.h"
#include "..\GameObject\human.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\explosion.h"
#include "..\App\audio.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\feet.h"
#include "..\GameObject\timeFade.h"
#include "..\GameObject\camera.h"
#include "..\App\model.h"
#include "..\GameObject\fence.h"

Model* Human::m_Model{};
Audio* Human::m_SE_Kick{};
Audio* Human::m_SE_Critical{};
Audio* Human::m_SE_Make{};

const int INITIAL_LIFE = 4;
const float INITIAL_SPEED = 0.08f;
const float INITIAL_SCALE_Y = 0.01f;
const int INITIAL_DEATH_TIME = 1400;
const bool INITIAL_ANIME_PAUSE = true;
const int INITIAL_ANIME_TIME = 0;
const float INITIAL_ANIME_ROTATION_X = 0.0f;
const float INITIAL_ORIENTATION_TIME = 0.0f;
const float INITIAL_NEXT_ROT_TIME = 0.0f;
const int INITIAL_NEXT_ROTATION = 1;
const int INITIAL_MAKING_TIME = 0;
const int INITIAL_KNOCK_BACK_TIME = 0;
const int INITIAL_STUN_TIME = 0;
const bool INITIAL_DELETE_INIT = false;
const int INITIAL_FEET_DROP = 4;
const float INITIAL_DEATH_STAGING = 0.14f;
const D3DXVECTOR3 INITIAL_BAR_SCALE = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
const float INITIAL_HP_BAR_POS_Y = 1.8f;
const float INITIAL_TRACKING = 10.0f;

const int MAKING_TIME = 30;
const int MAKING_COUNT = 4;
const int DROP_RATE = 20;
const int APPLE_RATE = 20;
const int KNOCK_BACK_TIME = 14;
const int STUN_TIME = 60;
const float TARGET_LENGTH = 3.0f;
const float GRAVITY = 0.015f;
const int SET_MAKING_INVALID_TIME = 180; // 柵作れない時間

void Human::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\human.obj");
	m_SE_Kick = new Audio();
	m_SE_Kick->Load("asset\\audio\\小キックb.wav");
	m_SE_Critical = new Audio();
	m_SE_Critical->Load("asset\\audio\\大パンチ.wav");
	m_SE_Make = new Audio();
	m_SE_Make->Load("asset\\audio\\木材に釘を打つ.wav");
}

void Human::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Human::Init()
{
	CharacterObject::Init();

	m_Life = INITIAL_LIFE;
	m_FullLife = INITIAL_LIFE;
	m_Speed = INITIAL_SPEED;
	m_Scale.y = INITIAL_SCALE_Y;
	m_Rotation.y = frand() * 2 * D3DX_PI;

	m_DeathTime = INITIAL_DEATH_TIME;
	m_AnimePause = INITIAL_ANIME_PAUSE;
	m_AnimeTime = INITIAL_ANIME_TIME;
	m_AnimeRotationX = INITIAL_ANIME_ROTATION_X;
	m_OrientationTime = INITIAL_ORIENTATION_TIME;
	m_NextRotTime = INITIAL_NEXT_ROT_TIME;
	m_NextRot = INITIAL_NEXT_ROTATION;
	m_MakingTime = INITIAL_MAKING_TIME;
	m_DeathStaging = INITIAL_DEATH_STAGING;
	m_KnockBackTime = INITIAL_KNOCK_BACK_TIME;
	m_StunTime = INITIAL_STUN_TIME;
	m_DeleteInit = INITIAL_DELETE_INIT;

	m_FeetDrop = INITIAL_FEET_DROP;
	m_BarScale = INITIAL_BAR_SCALE;
	m_HpBarPosY = INITIAL_HP_BAR_POS_Y;
	m_Tracking = INITIAL_TRACKING;


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_Shadow = AddComponent<Shadow>();
}

void Human::Uninit()
{
	CharacterObject::Uninit();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Human::Update()
{
	CharacterObject::Update();
	Scene* scene = Manager::GetScene();

	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);

	//自動削除まで
	m_DeathTime--;
	if (m_DeathTime <= 0) { UpdateDelete(); }
	else
	{
		SmoothAppearance(true);
	}

	//重力
	m_Velocity.y -= GRAVITY;

	//ノックバック
	KnockBack();

	//障害物との衝突判定
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//移動
	m_Position += m_Velocity;

	//HPが最大HPと一緒じゃなければライフバー表示
	if (m_FullLife != m_Life) { m_HpBarS->SetDraw(true); }

	//影の移動
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
	m_Shadow->SetScale(D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z));

	//HPバー表示
	D3DXVECTOR3 HpBarPosition = m_Position;
	HpBarPosition.y += m_HpBarPosY;
	m_HpBarS->SetPosition(HpBarPosition);

	//接地
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	//疑似アニメ
	Anime();
}

void Human::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera->CheckView(m_Position, m_Scale)) { return; }

	GameObject::Draw();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x + m_AnimeRotationX, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->DrawColor(m_Color, m_TextureEnable);
}

void Human::UpdateFree()
{
	//向き変更
	m_OrientationTime += 0.1f;
	if (m_OrientationTime > m_NextRotTime) { m_Rotation.y += (0.02f * m_NextRot); }
	if (m_OrientationTime > m_NextRotTime + 5.0f)
	{
		if (irand(1, 2) == 1) {
			m_NextRot *= -1;
		}
		m_NextRotTime = frand() * 30.0f + 10.0f;
		m_OrientationTime = 0.0f;
	}

	//移動
	m_Velocity.x = GetForward().x * m_Speed / 2;
	m_Velocity.z = GetForward().z * m_Speed / 2;

	//プレイヤーが範囲内であれば
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	float plength = D3DXVec3Length(&direction);

	//追尾範囲に入るか夜であれば
	TimeFade* timeFade = scene->GetGameObject<TimeFade>();
	if (plength < m_Tracking) { m_HumanState = HUMAN_STATE::TARGETING; }
	if (timeFade->GetTimeZone()) { m_HumanState = HUMAN_STATE::NIGHT; }
}

void Human::UpdateTargeting()
{
	Scene* scene = Manager::GetScene();
	TimeFade* timeFade = scene->GetGameObject<TimeFade>();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	float plength = D3DXVec3Length(&direction);

	//仲間羊がいれば優先的に
	auto follows = scene->GetGameObjects<Follow>();
	Follow* nearestFollow = nullptr;
	float nearestDistance = FLT_MAX;
	for (Follow* follow : follows)
	{
		D3DXVECTOR3 position = follow->GetPosition();
		D3DXVECTOR3 scale = follow->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		float length = D3DXVec3Length(&direction);

		if (length < m_Tracking && follow->GetState() != FOLLOW_STATE::FREE && m_HumanState != HUMAN_STATE::DAMAGE)
		{
			//今までのfor分の中で一番近い座標にいたなら向きを更新
			if (length < nearestDistance)
			{
				nearestDistance = length;
				nearestFollow = follow;
			}
		}
	}

	//一番近いフォローオブジェクトが見つかったら向き変更
	if (nearestFollow != nullptr)
	{
		m_Rotation.y = atan2f(nearestFollow->GetPosition().x - m_Position.x, nearestFollow->GetPosition().z - m_Position.z);
	}
	//でなければプレイヤーの方に向き変更
	else { m_Rotation.y = atan2f((player->GetPosition().x) - m_Position.x, (player->GetPosition().z) - m_Position.z); }

	//柵を作れない時間を減らす
	m_MakingInvalidTime--;
	if (m_MakingInvalidTime <= 0) { m_MakingInvalidTime = 0; }

	//移動処理(時間)
	bool timeFede = timeFade->GetTimeZone();
	m_Velocity.x = GetForward().x * m_Speed;
	m_Velocity.z = GetForward().z * m_Speed;

	//無視範囲を超えたら
	if (plength > m_Tracking) { m_HumanState = HUMAN_STATE::FREE; }//追尾しない
	//夜なら
	if (timeFede) { m_HumanState = HUMAN_STATE::NIGHT; }
}

void Human::UpdateDelete()
{
	//ぬるぬる消滅
	m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_HpBarS->SetDraw(false);
	SmoothAppearance(false);
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void Human::UpdateMaking()
{
	//移動しない
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_MakingTime++;
	if (m_MakingTime % MAKING_TIME == 0)
	{
		//オブジェクト発生位置調整(お邪魔者の前)
		D3DXVECTOR3 objectPosition = m_Position;
		objectPosition.x += GetForward().x;
		objectPosition.z += GetForward().z;
		
		//効果音とエフェクト
		m_SE_Make->Play(1.0f);
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Explosion>(1)->SetPosition(objectPosition);//爆発エフェクト


		if (m_MakingTime >= MAKING_TIME * MAKING_COUNT)
		{
			m_MakingTime = 0;
			m_MakingInvalidTime = SET_MAKING_INVALID_TIME;
			m_HumanState = HUMAN_STATE::FREE;

			//柵発生
			Make(objectPosition);
		}
	}
}

void Human::UpdateAlive()
{
	CharacterObject::UpdateAlive();

	switch (m_HumanState)
	{
	case HUMAN_STATE::FREE:
		UpdateFree();
		break;
	case HUMAN_STATE::MAKING:
		UpdateMaking();
		break;
	case HUMAN_STATE::DAMAGE:
		UpdateDamage();
		break;
	case HUMAN_STATE::NIGHT:
		UpdateNight();
		break;
	case HUMAN_STATE::TARGETING:
		UpdateTargeting();
		break;
	}
}

void Human::UpdateDead()
{
	Scene* scene = Manager::GetScene();

	if (!m_DeleteInit)
	{
		m_SE_Critical->Play(1.0f);
		m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_HpBarS->SetDraw(false);
		m_DeleteInit = true;

		for (int i = 0; i < m_FeetDrop; i++)
		{
			Feet* feet = scene->AddGameObject<Feet>(1);
			feet->SetPosition(m_Position);
			feet->SetDrop();
		}
	}
	m_Rotation.z += m_DeathStaging / 2.0f;
	m_Rotation.y -= m_DeathStaging;
	m_Position.y += 0.5f;
	m_Scale.x -= 0.1f; m_Scale.y -= 0.1f; m_Scale.z -= 0.1f;
	m_DeathStaging -= 0.01f;

	if (m_Scale.y <= 0.0f)
	{
		scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
		SetDestroy();
	}

}

void Human::UpdateDamage()
{
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	Escape();

	//攻撃受けたときの処理
	m_StunTime--;
	if (m_StunTime <= 0)
	{
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_HumanState = HUMAN_STATE::FREE;
	}
}

void Human::UpdateNight()
{
	Escape();

	Scene* scene = Manager::GetScene();
	TimeFade* timeFade = scene->GetGameObject<TimeFade>();
	if (!timeFade->GetTimeZone()) { m_HumanState = HUMAN_STATE::FREE; }
}

void Human::KnockBack()
{
	//後退
	if (m_KnockBackTime >= 1)
	{
		m_KnockBackTime--;
		m_Velocity.x += (m_PLForward.x * (m_KnockBackTime * 0.02f));
		m_Velocity.z += (m_PLForward.z * (m_KnockBackTime * 0.02f));
		if (m_KnockBackTime <= 0) { m_KnockBackTime = 0; }
	}
}

void Human::Escape()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//プレイヤーの反対を向く
	m_Rotation.y = atan2f(m_Position.x - player->GetPosition().x, m_Position.z - player->GetPosition().z);

	//移動処理(時間)
	m_Velocity.x = GetForward().x * (m_Speed * 2.0f);
	m_Velocity.z = GetForward().z * (m_Speed * 2.0f);
}

void Human::Anime()
{
	m_AnimeTime++;
	if (m_AnimeTime > 7)
	{
		//傾ける
		if (m_HumanState == HUMAN_STATE::TARGETING || m_HumanState == HUMAN_STATE::MAKING) { m_AnimeRotationX = (0.16f * m_AnimePause); }
		else { m_AnimeRotationX = (0.04f * m_AnimePause); }

		//リセット
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
	}
}

void Human::Make(D3DXVECTOR3 objectPosition)
{
	Scene* scene = Manager::GetScene();

	//柵発生
	Fence* fence = scene->AddGameObject<Fence>(1);
	fence->SetPosition(objectPosition);
	fence->SetRotation(m_Rotation);
}

void Human::SetDamageMove()
{
	CharacterObject::SetDamageMove();

	m_StunTime = STUN_TIME;
	m_HumanState = HUMAN_STATE::DAMAGE;

	m_KnockBackTime = KNOCK_BACK_TIME;
	m_Life--;
	m_SE_Kick->Play(1.0f);
	m_MakingTime = 0;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	m_PLForward = player->GetForward();
	scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
}

void Human::Collision(float& groundHeight)
{
	if (m_CharacterState == CHARACTER_STATE::DEAD) { return; };

	CharacterObject::Collision(groundHeight);
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//キャラクターオブジェクトとの接触判定
	auto characterObjects = scene->GetGameObjects<CharacterObject>();
	for (CharacterObject* characterObject : characterObjects)
	{
		//自分自身の場合はスキップ
		if (characterObject == this) { continue; }

		D3DXVECTOR3 position = characterObject->GetPosition();
		D3DXVECTOR3 scale = characterObject->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		float length = D3DXVec3Length(&direction);

		//どのオブジェクトか判定
		Player* playerObject = dynamic_cast<Player*>(characterObject);
		Follow* followObject = dynamic_cast<Follow*>(characterObject);

		if (length < scale.x && length < scale.y && length < scale.z)
		{
			//オブジェクト問わずズレる
			m_Position.x += (m_Position.x - characterObject->GetPosition().x) * 0.04f;
			m_Position.z += (m_Position.z - characterObject->GetPosition().z) * 0.04f;

			//インスタンスがPlayerであるか判定
			if (playerObject)
			{
				if (playerObject->GetAttackStop() <= 0 && playerObject->GetInvincibleTime() <= 0)
				{
					//プレイヤーがダッシュ時にぶつかった場合
					if (player->GetCharacterState() == CHARACTER_STATE::ALIVE &&
						player->GetPlayerState() == PLAYER_STATE::DASH)
					{
						SetDamageMove();
						playerObject->SetKnockBack();
						player->AddCombo(1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
					}
				}
			}

			//インスタンスがFollowであるか判定
			else if (followObject)
			{
				if (followObject->GetAttackStop() <= 0)
				{
					if (followObject->GetState() == FOLLOW_STATE::DASH)
					{
						SetDamageMove();
						followObject->SetKnockBack();
						player->AddCombo(1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
					}
				}
			}
		}

		//枠内にプレイヤーいて夜じゃなくてメイキングしてなければ
		if (length < TARGET_LENGTH && m_StunTime <= 0 && m_HumanState != HUMAN_STATE::MAKING && m_HumanState != HUMAN_STATE::NIGHT && m_MakingInvalidTime <= 0)
		{
			if (playerObject || (followObject && followObject->GetState() != FOLLOW_STATE::FREE)) { m_HumanState = HUMAN_STATE::MAKING; }
		}
	}


	// 破壊可ブロック
	auto breakObjects = scene->GetGameObjects<BreakObject>();
	for (BreakObject* breakObject : breakObjects)
	{
		if (breakObject->GetState() != BREAKOBJECT_STATE::DEATH)
		{

			D3DXVECTOR3 position = breakObject->GetPosition();
			D3DXVECTOR3 scale = breakObject->GetScale();
			D3DXVECTOR3 right = breakObject->GetRight();
			D3DXVECTOR3 forward = breakObject->GetForward();
			D3DXVECTOR3 up = breakObject->GetUp();
			D3DXVECTOR3 direction = m_Position - position;
			float obbX = D3DXVec3Dot(&direction, &right);
			float obbZ = D3DXVec3Dot(&direction, &forward);
			float obbY = D3DXVec3Dot(&direction, &up);

			// OBB
			if (fabs(obbX) < scale.x && fabs(obbZ) < scale.z && fabs(obbY) < scale.y)
			{
				D3DXVECTOR3 penetration = D3DXVECTOR3(fabsf(scale.x) - fabsf(obbX), fabsf(scale.y) - fabsf(obbY), fabsf(scale.z) - fabsf(obbZ));

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
	}
}

