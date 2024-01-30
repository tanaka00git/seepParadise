#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\GameObject\characterObject.h"
#include "..\GameObject\human.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\fence.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\explosion.h"
#include "..\App\audio.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\coin.h"
#include "..\GameObject\apple.h"
#include "..\GameObject\itemEye.h"
#include "..\GameObject\itemSpeed.h"
#include "..\GameObject\itemLife.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\timeFade.h"
#include "..\GameObject\camera.h"
#include "..\App\model.h"

Model* Human::m_Model{};
Audio* Human::m_SE_Kick{};

#define LIFE 4
#define SPEED 0.04f
#define EATING_TIME 45
#define EATING_COUNT 5
#define DROP_RATE 20
#define APPLE_RATE 20
#define GIVE_ATTACK_STOP 20
#define KNOCK_BACK_TIME 14
#define STUN_TIME 100

void Human::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\human.obj");
	m_SE_Kick = new Audio();
	m_SE_Kick->Load("asset\\audio\\小キックb.wav");
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

	m_Life = LIFE;
	m_FullLife = LIFE;
	m_Speed = SPEED;
	m_Scale.y = 0.01f;

	m_Rotation.y = frand() * 2 * D3DX_PI;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_Shadow = AddComponent<Shadow>();

	m_HpBarS = AddComponent<HpBarS>();
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
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
	m_DaathTime--;
	if (m_DaathTime <= 0) { UpdateDelete(); }
	else
	{
		m_Scale.x += 0.05f;
		m_Scale.y += 0.05f;
		m_Scale.z += 0.05f;
		if (m_Scale.x >= m_OriginalScale.x) { m_Scale.x = m_OriginalScale.x; }
		if (m_Scale.y >= m_OriginalScale.y) { m_Scale.y = m_OriginalScale.y; }
		if (m_Scale.z >= m_OriginalScale.z) { m_Scale.z = m_OriginalScale.z; }
	}

	//重力
	m_Velocity.y -= 0.015f;

	//ノックバック
	KnockBack();

	//障害物との衝突判定
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//移動
	m_Position += m_Velocity;

	//HPが最大HPと一緒じゃなければライフバー表示
	if (m_FullLife != m_Life) { m_HpBarS->SetScale(m_BarScale); }
	else { m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); }

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

	//ダメージフラッシュ
	DamageFlash();
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
		int a = irand(0, 2);
		if (a == 1) {
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
	if (plength < m_Tracking || timeFade->GetTimeZone()) { m_HumanState = HUMAN_STATE::TARGETING; }
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

	//移動処理(時間)
	bool timeFede = timeFade->GetTimeZone();
	m_Velocity.x = GetForward().x * (m_Speed + (timeFede * m_Speed));
	m_Velocity.z = GetForward().z * (m_Speed + (timeFede * m_Speed));

	//災害狼専用イベント
	if (m_Disaster) { DisasterMove(); }

	//無視範囲を超えたら
	if (plength > m_Tracking) { m_HumanState = HUMAN_STATE::FREE; }//追尾しない
}

void Human::UpdateDelete()
{
	//ぬるぬる消滅
	m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Scale.x -= 0.05f; m_Scale.y -= 0.05f; m_Scale.z -= 0.05f;
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void Human::UpdateEating()
{
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_EatStop++;
	if (m_EatStop % EATING_TIME == 0)
	{
		if (m_EatStop >= EATING_TIME * EATING_COUNT)
		{
			m_EatStop = 0;
			m_HumanState = HUMAN_STATE::FREE;

			//柵発生位置調整(お邪魔者の前)
			D3DXVECTOR3 objectPosition = m_Position;
			objectPosition.x += GetForward().x * 0.8f;
			objectPosition.z += GetForward().z * 0.8f;

			Scene* scene = Manager::GetScene();
			Fence* fence = scene->AddGameObject<Fence>(1);
			fence->SetPosition(objectPosition);
			fence->SetRotation(m_Rotation);
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
		UpdateEating();
		break;
	case HUMAN_STATE::DAMAGE:
		UpdateDamage();
		break;
	case HUMAN_STATE::TARGETING:
		UpdateTargeting();
		break;
	}
}

void Human::UpdateDeath()
{
	Scene* scene = Manager::GetScene();

	if (!m_DeleteInit)
	{
		m_SE_Kick->Play(1.0f);
		m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_DeleteInit = true;

		for (int i = 0; i < m_CoinDrop; i++)
		{
			Coin* coin = scene->AddGameObject<Coin>(1);
			coin->SetPosition(m_Position);
			coin->SetDrop();
		}

		//リンゴドロップ
		if (m_Item)
		{
			Apple* apple = scene->AddGameObject<Apple>(1);
			apple->SetPosition(m_Position);
			apple->SetDrop();
		}

		//アイテムドロップ
		int a = irand(0, DROP_RATE - 1);
		if (a == 0) {
			a = irand(0, 2);
			if (a == 0) {
				ItemSpeed* itemSpeed = scene->AddGameObject<ItemSpeed>(1);
				itemSpeed->SetPosition(m_Position);
				itemSpeed->SetDrop();
			}
			else if (a == 1) {
				ItemLife* itemLife = scene->AddGameObject<ItemLife>(1);
				itemLife->SetPosition(m_Position);
				itemLife->SetDrop();
			}
			else if (a == 2) {
				ItemEye* itemEye = scene->AddGameObject<ItemEye>(1);
				itemEye->SetPosition(m_Position);
				itemEye->SetDrop();

			}
		}
	}

	m_Rotation.z += m_Death / 2.0f;
	m_Rotation.y -= m_Death;
	m_Position.y += 0.5f;
	m_Scale.x -= 0.1f; m_Scale.y -= 0.1f; m_Scale.z -= 0.1f;
	m_Death -= 0.01f;

	if (m_Scale.y <= 0.0f)
	{
		scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
		SetDestroy();
	}

}

void Human::UpdateDamage()
{
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	m_Velocity *= 0;
	m_Rotation.y += 0.2f;

	//攻撃受けたときの処理
	m_StunTime--;
	if (m_StunTime <= 0)
	{
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_HumanState = HUMAN_STATE::FREE;
	}
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

void Human::DisasterMove()
{
	//自動消滅しないように適当な値で上書き
	m_DaathTime = 1000;

	//一定時間経つとオートで敵を生み出す
	//Scene* scene = Manager::GetScene();
	//m_DisasterCount++;
	//if (m_DisasterCount >= 150)
	//{
	//	Wolf* wolf = scene->AddGameObject<Wolf>(1);
	//	wolf->SetEnemyData(1);
	//	wolf->SetPosition(m_Position);
	//	m_DisasterCount = 0;
	//}
}

void Human::Anime()
{
	m_AnimeTime++;
	if (m_AnimeTime > 7)
	{
		//傾ける
		if (m_HumanState == HUMAN_STATE::TARGETING) { m_AnimeRotationX = (0.16f * m_AnimePause); }
		else { m_AnimeRotationX = (0.04f * m_AnimePause); }

		//リセット
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
	}
}

void Human::SetDamageMove()
{
	m_StunTime = STUN_TIME;
	m_HumanState = HUMAN_STATE::DAMAGE;

	m_Velocity.y = 0.1f;
	m_KnockBackTime = KNOCK_BACK_TIME;
	m_Life--;
	m_SE_Kick->Play(1.0f);
	m_ColorChange = 5;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	m_PLForward = player->GetForward();
	scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト

	Camera* camera = scene->GetGameObject<Camera>();
	camera->SetShake(0.1f, 0.0f);
}

void Human::Collision(float& groundHeight)
{
	if (m_CharacterState == CHARACTER_STATE::DEAD) { return; };

	CharacterObject::Collision(groundHeight);

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	if (player->GetInvincibleTime() <= 0)
	{
		//プレイヤーの距離を取得	
		D3DXVECTOR3 direction = m_Position - player->GetPosition();
		D3DXVECTOR3 pscale = player->GetScale();
		float plength = D3DXVec3Length(&direction);

		if (player->GetAttackStop() <= 0)
		{
			D3DXVECTOR3 scale = player->GetScale();
			if (plength < scale.x && plength < scale.y && plength < scale.z)
			{
				//プレイヤーがダッシュ時にぶつかった場合
				if (player->GetCharacterState() == CHARACTER_STATE::ALIVE &&
					player->GetPlayerState() == PLAYER_STATE::DASH)
				{
					SetDamageMove();
					player->SetAttackStop(GIVE_ATTACK_STOP);
					player->AddCombo(1);
					scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
				}
				//普通にぶつかった場合
				else if (m_StunTime <= 0 && m_HumanState != HUMAN_STATE::MAKING)
				{
					m_HumanState = HUMAN_STATE::MAKING;
				}
			}
		}

	}

	auto follows = scene->GetGameObjects<Follow>();
	for (Follow* follow : follows) {
		D3DXVECTOR3 position = follow->GetPosition();
		D3DXVECTOR3 scale = follow->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		float length = D3DXVec3Length(&direction);

		if (length < scale.x && length < scale.y && length < scale.z)
		{
			if (follow->GetAttackStop() <= 0)
			{
				if (follow->GetState() == FOLLOW_STATE::DASH)
				{
					SetDamageMove();
					follow->SetAttackStop(GIVE_ATTACK_STOP);
					player->AddCombo(1);
					scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト

				}
				else if (m_StunTime <= 0 && m_HumanState != HUMAN_STATE::MAKING)
				{
					m_HumanState = HUMAN_STATE::MAKING;
				}
			}
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

			// 影の高さ設定
			if (fabs(obbX) < scale.x && fabs(obbZ) < scale.z && m_Position.y > position.y + scale.y - 0.5f)
			{
				groundHeight = max(groundHeight, position.y + scale.y);
			}

			// OBB
			if (fabs(obbX) < scale.x && fabs(obbZ) < scale.z && fabs(obbY) < scale.y)
			{
				D3DXVECTOR3 penetration = D3DXVECTOR3(scale.x - fabs(obbX), scale.y - fabs(obbY), scale.z - fabs(obbZ));

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
