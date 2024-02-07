#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\GameObject\characterObject.h"
#include "..\GameObject\wolf.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
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

Model*Wolf::m_Model{};
Model*Wolf::m_ModelApple{};
Audio*Wolf::m_SE_Eat{};
Audio*Wolf::m_SE_Kick{};
Audio*Wolf::m_SE_Critical{};

#define EATING_TIME 45
#define DROP_RATE 20
#define APPLE_RATE 20
#define KNOCK_BACK_TIME 14
#define STUN_TIME 240
#define GRAVITY 0.015f

void Wolf::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\wolf_a.obj");

	m_ModelApple = new Model();
	m_ModelApple->Load("asset\\model\\wolfClown3.obj");
	m_SE_Eat = new Audio();
	m_SE_Eat->Load("asset\\audio\\eat3b.wav");
	m_SE_Kick = new Audio();
	m_SE_Kick->Load("asset\\audio\\小キックb.wav");
	m_SE_Critical = new Audio();
	m_SE_Critical->Load("asset\\audio\\大パンチ.wav");
}

void Wolf::Unload()
{
	m_Model->Unload();
	m_ModelApple->Unload();
	delete m_Model;
	delete m_ModelApple;
	m_Model = nullptr;
	m_ModelApple = nullptr;

}

void Wolf::Init()
{
	CharacterObject::Init();

	m_Scale.y = 0.01f;
	m_Rotation.y = frand() * 2 * D3DX_PI;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
	
	m_Shadow = AddComponent<Shadow>();
	m_HpBarS = AddComponent<HpBarS>();
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
}

void Wolf::Uninit()
{
	CharacterObject::Uninit();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Wolf::Update()
{
	CharacterObject::Update();
	Scene* scene = Manager::GetScene();

	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);

	//自動削除まで
	m_DaathTime --;
	if (m_DaathTime <= 0){UpdateDelete();}
	else 
	{
		m_Scale.x += m_OriginalScale.x / 20;
		m_Scale.y += m_OriginalScale.y / 20;
		m_Scale.z += m_OriginalScale.z / 20;
		if (m_Scale.x >= m_OriginalScale.x) { m_Scale.x = m_OriginalScale.x; }
		if (m_Scale.y >= m_OriginalScale.y) { m_Scale.y = m_OriginalScale.y; }
		if (m_Scale.z >= m_OriginalScale.z) { m_Scale.z = m_OriginalScale.z; }
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
}

void Wolf::Draw()
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

	if (m_Item) { m_ModelApple->DrawColor(m_Color, m_TextureEnable); }
}

void Wolf::UpdateFree()
{
	//向き変更
	m_OrientationTime += 0.1f;
	if (m_OrientationTime > m_NextRotTime){m_Rotation.y += (0.02f * m_NextRot);}
	if (m_OrientationTime > m_NextRotTime + 5.0f) 
	{
		int a = irand(0,2);
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
	if (plength < m_Tracking || timeFade->GetTimeZone()) {m_WolfState = WOLF_STATE::TARGETING;}
}

void Wolf::UpdateTargeting()
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

		if (length < m_Tracking && follow->GetState() != FOLLOW_STATE::FREE && m_WolfState != WOLF_STATE::DAMAGE)
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

	//無視範囲を超えたら
	if (plength > m_Tracking) { m_WolfState = WOLF_STATE::FREE; }//追尾しない
}

void Wolf::UpdateDelete()
{
	//ぬるぬる消滅
	m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Scale.x -= m_OriginalScale.y / 20; m_Scale.y -= m_OriginalScale.y / 20; m_Scale.z -= m_OriginalScale.y / 20;
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void Wolf::UpdateEating()
{
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_EatStop ++;
	if (m_EatStop % EATING_TIME == 0)
	{ 
		if(m_EatStop >= EATING_TIME * m_BiteCount)
		{
			m_EatStop = 0;
			m_WolfState = WOLF_STATE::FREE;
		}
		else { m_SE_Eat->Play(1.0f); }
	}
}

void Wolf::UpdateAlive()
{
	CharacterObject::UpdateAlive();

	switch (m_WolfState)
	{
	case WOLF_STATE::FREE:
		UpdateFree();
		break;
	case WOLF_STATE::EATING:
		UpdateEating();
		break;
	case WOLF_STATE::DAMAGE:
		UpdateDamage();
		break;
	case WOLF_STATE::SUPER_CHARGE:
		UpdateSuperCharge();
		break;
	case WOLF_STATE::SUPER_ATTACK:
		UpdateSuperAttack();
		break;
	case WOLF_STATE::TARGETING:
		UpdateTargeting();
		break;
	}

}

void Wolf::UpdateDead()
{
	Scene* scene = Manager::GetScene();

	if (!m_DeleteInit)
	{
		m_SE_Critical->Play(1.0f);
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
		int a = irand(0,DROP_RATE-1);
		if (a == 0) {
			a = irand(0,2);
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

void Wolf::UpdateDamage()
{
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	m_Velocity *= 0;
	m_Rotation.y += 0.2f;

	//攻撃受けたときの処理
	m_StunTime --;
	if (m_StunTime <= 0) 
	{
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_WolfState = WOLF_STATE::FREE;
	}
}

void Wolf::KnockBack()
{
	//後退
	if (m_KnockBackTime >= 1) 
	{
		m_KnockBackTime --;
		m_Velocity.x += (m_PLForward.x * (m_KnockBackTime * 0.03f));
		m_Velocity.z += (m_PLForward.z * (m_KnockBackTime * 0.03f));
		if (m_KnockBackTime <= 0) { m_KnockBackTime = 0; }
	}
}

void Wolf::Anime()
{
	m_AnimeTime++;
	if (m_AnimeTime > 7)
	{
		//傾ける
		if (m_WolfState == WOLF_STATE::TARGETING || m_WolfState == WOLF_STATE::EATING) { m_AnimeRotationX = (0.16f * m_AnimePause); }
		else { m_AnimeRotationX = (0.04f * m_AnimePause); }

		//リセット
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
	}
}

void Wolf::SetDamageMove()
{
	CharacterObject::SetDamageMove();

	//たくさん攻撃を与えるとダメージ状態になる
	m_StanGuardCount++;
	if (m_StanGuardCount >= m_StanGuard) 
	{
		m_StanGuardCount = 0;
		m_StunTime = STUN_TIME;
		m_WolfState = WOLF_STATE::DAMAGE; 
	}

	m_KnockBackTime = KNOCK_BACK_TIME;
	m_SE_Kick->Play(1.0f);

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	m_PLForward = player->GetForward();
	scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト

	Camera*camera = scene->GetGameObject<Camera>();
	camera->SetShake(0.2f, 0.0f);
}

void Wolf::SetPosEnemyData(D3DXVECTOR3 Position, int Num)
{
	m_Position = Position;
	SetEnemyData(Num);
}

void Wolf::SetEnemyData(int data)
{
	m_Data = data;

	//低確率でリンゴを頭に乗せる
	int dropPercent = irand(0,19);
	if (dropPercent == 0) { m_Item = true; }

	//ステータス
	m_FullLife = 4 * data;
	m_CoinDrop = data;
	m_StanGuard = m_FullLife / 2;
	m_OriginalScale = D3DXVECTOR3(0.5f * data, 0.5f * data, 0.5f * data);	//キャラのサイズ
	m_Speed = 0.03f + (0.0025f * data);

	if (m_Data == 1)
	{
		m_BiteCount = 5;
		m_BarScale = D3DXVECTOR3(0.5f, 0.7f, 0.7f);		//HPバーのサイズ
		m_HpBarPosY = 1.8f;
	}
	else if (m_Data == 2)
	{
		m_BiteCount = 4;
		m_BarScale = D3DXVECTOR3(0.7f, 0.7f, 0.7f);	
		m_HpBarPosY = 1.8f;
	}
	else if (m_Data == 3)
	{
		m_BiteCount = 3;
		m_BarScale = D3DXVECTOR3(1.0f, 0.7f, 0.7f);
		m_HpBarPosY = 2.2f;
	}
	else if (m_Data == 4)
	{
		m_BiteCount = 3;
		m_BarScale = D3DXVECTOR3(1.4f, 0.7f, 0.7f);
		m_HpBarPosY = 2.8f;
	}
	m_Life = m_FullLife;
	
}

void Wolf::Collision(float& groundHeight)
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
		if (length < scale.x && length < scale.y && length < scale.z)
		{
			//インスタンスがPlayerであるか判定
			if (Player* playerObject = dynamic_cast<Player*>(characterObject))
			{
				if (playerObject->GetAttackStop() <= 0 && playerObject->GetInvincibleTime() <= 0)
				{
					//プレイヤーがダッシュ時にぶつかった場合
					if (m_WolfState == WOLF_STATE::SUPER_ATTACK)
					{
						playerObject->SetDamageMove();
						m_SE_Eat->Play(1.0f);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
					}
					else if (playerObject->GetCharacterState() == CHARACTER_STATE::ALIVE &&
						playerObject->GetPlayerState() == PLAYER_STATE::DASH)
					{
						SetDamageMove();
						playerObject->SetKnockBack();
						playerObject->AddCombo(1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
					}
					//普通にぶつかった場合
					else if (m_StunTime <= 0 && m_WolfState != WOLF_STATE::EATING)
					{
						m_WolfState = WOLF_STATE::EATING;
						playerObject->SetDamageMove();
						m_SE_Eat->Play(1.0f);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
					}
				}
			}

			//インスタンスがFollowであるか判定
			else if (Follow* followObject = dynamic_cast<Follow*>(characterObject))
			{
				if (followObject->GetAttackStop() <= 0)
				{
					if (m_WolfState == WOLF_STATE::SUPER_ATTACK)
					{
						m_SE_Eat->Play(1.0f);
						followObject->AddLife(-1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
					}
					else if (followObject->GetState() == FOLLOW_STATE::DASH)
					{
						SetDamageMove();
						followObject->SetKnockBack();
						player->AddCombo(1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト

					}
					else if (m_StunTime <= 0 && m_WolfState != WOLF_STATE::EATING)
					{
						m_WolfState = WOLF_STATE::EATING;
						m_SE_Eat->Play(1.0f);
						followObject->AddLife(-1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//爆発エフェクト
					}
				}
			}
			//オブジェクト問わずズレる
			m_Position.x += (m_Position.x - characterObject->GetPosition().x) * 0.04f;
			m_Position.z += (m_Position.z - characterObject->GetPosition().z) * 0.04f;
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