#include "..\App\main.h"
#include "characterObject.h"
#include "..\App\manager.h"
#include "..\Scene\scene.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\box.h"

#define INIT_JUMP 0.25f			//出現のジャンプの高さ

void CharacterObject::Init()
{
	GameObject::Init();
}

void CharacterObject::Uninit()
{
	GameObject::Uninit();
}

void CharacterObject::Update()
{
	GameObject::Update();

	switch (m_CharacterState)
	{
	case CHARACTER_STATE::ALIVE:
		UpdateAlive();
		break;
	case CHARACTER_STATE::DEAD:
		UpdateDeath();
		break;
	case CHARACTER_STATE::UNUSED:
		UpdateUnused();
		break;
	}

	//ダメージフラッシュ
	DamageFlash();
}

void CharacterObject::Draw()
{
}

void CharacterObject::UpdateAlive()
{
	//生死時の処理
	if (m_Life <= 0) { m_CharacterState = CHARACTER_STATE::DEAD; }
}

void CharacterObject::UpdateDeath()
{
}

void CharacterObject::UpdateUnused()
{
	m_Scale *= 0.0f;
}

void CharacterObject::DamageFlash()
{
	if (m_DamageFlashTime > 0)
	{
		m_DamageFlashTime--;
		m_TextureEnable = false;
	}
	else { m_TextureEnable = true; }
}

void CharacterObject::SetDrop()
{
	m_Velocity.y = INIT_JUMP;
	m_Rotation.y = frand() * 3.1415f;
	m_Velocity.x = GetForward().x * (frand() * 0.08f);
	m_Velocity.z = GetForward().z * (frand() * 0.08f);
}

void CharacterObject::Collision(float& groundHeight)
{
	Scene* scene = Manager::GetScene();

	//円柱
	auto cylinders = scene->GetGameObjects<Cylinder>();		//円柱のリストを取得
	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		scale.x *= 1.2f, scale.z *= 1.2f;					//半径の調整
		D3DXVECTOR3 direction = m_Position - position;		//円柱中心からキャラまでのベクトル
		float length = D3DXVec3Length(&direction);			//キャラと円柱中心の距離
		D3DXVECTOR3 up = cylinder->GetUp();					//円柱の上方向ベクトル
		float obbr = D3DXVec3Dot(&direction, &up);			//円柱の上方向への軸分離

		direction -= up * obbr;	//y軸の接触を無視するために、directionからupの成分を引く
		length = D3DXVec3Length(&direction); //新しいlengthを計算する

		//影
		if (length < scale.x && m_Position.y > position.y + scale.y - 0.5f)
		{
			groundHeight = max(groundHeight, position.y + scale.y);
		}
		// OBB
		if (length < scale.x && fabs(obbr) < scale.y)
		{
			// 円柱の上下方向への押し出し処理
			float penetrationH = scale.y - static_cast<float>(fabs(obbr));
			float penetrationX = scale.x - length;

			// 横から触れた場合の処理
			if (penetrationX < penetrationH)
			{
				// 円柱内に入った場合の処理
				D3DXVECTOR3 normalizedDirection = direction / length;
				float penetration = scale.x - length;

				// 入った方向に押し出し処理
				m_Position += penetration * normalizedDirection;
			}
			else
			{
				if (obbr > 0)
				{
					m_Position += penetrationH * up;
					m_Velocity.y = 0.0f;  // 上に乗ったら垂直速度を0にする
				}
				else
				{
					m_Position -= penetrationH * up;
					m_Velocity.y -= m_Velocity.y; // 下から触れたら垂直速度を反転する
				}
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
		float obbx = D3DXVec3Dot(&direction, &right);	//X軸分離方向キャラ距離
		float obbz = D3DXVec3Dot(&direction, &forward);	//Z軸分離方向キャラ距離
		float obby = D3DXVec3Dot(&direction, &up);		//Y軸分離方向キャラ距離

		//影の高さ設定
		if (fabs(obbx) < scale.x && fabs(obbz) < scale.z)
		{
			if (m_Position.y > position.y + scale.y - 0.5f) { groundHeight = max(groundHeight, position.y + scale.y); }
		}
		//OBB
		if (fabs(obbx) < scale.x && fabs(obbz) < scale.z && fabs(obby) < scale.y)
		{
			//キャラが直方体よりも上にいるかどうかの判定
			if (m_Position.y > position.y + scale.y)
			{
				groundHeight = position.y + scale.y;
			}
			D3DXVECTOR3 penetration = D3DXVECTOR3(scale.x - abs(obbx), scale.y - abs(obby), scale.z - abs(obbz));

			if (penetration.x < penetration.z && penetration.x < penetration.y)
			{
				if (obbx > 0) { m_Position += penetration.x * right; }
				else { m_Position -= penetration.x * right; }
			}
			else if (penetration.z < penetration.y)
			{
				if (obbz > 0) { m_Position += penetration.z * forward; }
				else { m_Position -= penetration.z * forward; }
			}
			else
			{
				if (obby > 0)
				{
					m_Position += penetration.z * up;
					m_Velocity.y = 0.0f;			//上に乗ったら垂直速度を0にする
				}
				else
				{
					m_Position -= penetration.y * up;
					m_Velocity.y = -m_Velocity.y;	//下から触れたら垂直速度を反転する
				}
			}
		}
	}

	if (m_Position.y < 0.0f && m_Velocity.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Velocity.y = 0.0f;
	}
}
