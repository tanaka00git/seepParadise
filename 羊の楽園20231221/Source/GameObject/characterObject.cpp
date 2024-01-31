#include "..\App\main.h"
#include "characterObject.h"
#include "..\App\manager.h"
#include "..\Scene\scene.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\box.h"

#define INIT_JUMP 0.25f			//�o���̃W�����v�̍���

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
		UpdateDead();
		break;
	case CHARACTER_STATE::UNUSED:
		UpdateUnused();
		break;
	}

	//�_���[�W�t���b�V��
	DamageFlash();
}

void CharacterObject::Draw()
{
}

void CharacterObject::UpdateAlive()
{
	//�������̏���
	if (m_Life <= 0) { m_CharacterState = CHARACTER_STATE::DEAD; }
}

void CharacterObject::UpdateDead()
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

	// �~��
	auto cylinders = scene->GetGameObjects<Cylinder>();    // �~���̃��X�g���擾
	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		scale.x *= 1.2f, scale.z *= 1.2f;					//���a�̒���
		D3DXVECTOR3 direction = m_Position - position;		//�~�����S����L�����܂ł̃x�N�g��
		D3DXVECTOR3 up = cylinder->GetUp();					//�~���̏�����x�N�g��
		float obbY = D3DXVec3Dot(&direction, &up);			//�~���̏�����ւ̎�����

		//�e�̍����𒲐����邽�ߗp
		direction -= up * obbY;								//y���̐ڐG�𖳎����邽�߂ɁAdirection����up�̐���������
		float length = D3DXVec3Length(&direction);			//�V����length���v�Z����i�L�����Ɖ~�����S�̋����j

		// �e
		if (length < scale.x && m_Position.y > position.y + scale.y - 0.5f)
		{
			groundHeight = max(groundHeight, position.y + scale.y);
		}
		//�����蔻��
		if (length < scale.x && fabs(obbY) < scale.y)
		{
			D3DXVECTOR3 normalizedDirection = direction / length;
			float penetrationY = scale.y - static_cast<float>(fabs(obbY));
			float penetrationX = scale.x - length;

			// �~�����ɓ������ꍇ�̏���
			if (penetrationX < penetrationY)
			{
				// �����������ɉ����o������
				m_Position += penetrationX * normalizedDirection;
			}
			else
			{
				m_Position += (obbY > 0) ? (penetrationY * up) : (-penetrationY * up);
				m_Velocity.y = (obbY > 0) ? 0.0f : -m_Velocity.y; // ��ɏ�����琂�����x��0�ɂ���A������G�ꂽ�甽�]����
			}
		}
	}

	//������
	auto boxs = scene->GetGameObjects<Box>();	//���X�g���擾
	for (Box* box : boxs)						//�͈�for���[�v
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();
		scale.y *= 1.8f;								//��������
		D3DXVECTOR3 right = box->GetRight();			//x������
		D3DXVECTOR3 forward = box->GetForward();		//z������
		D3DXVECTOR3 up = box->GetUp();					//y������
		D3DXVECTOR3 direction = m_Position - position;	//�����̂���L�����܂ł̕����x�N�g��
		float obbX = D3DXVec3Dot(&direction, &right);
		float obbZ = D3DXVec3Dot(&direction, &forward);
		float obbY = D3DXVec3Dot(&direction, &up);

		//�e�̍����ݒ�
		if (fabs(obbX) < scale.x && fabs(obbZ) < scale.z)
		{
			if (m_Position.y > position.y + scale.y - 0.5f) { groundHeight = max(groundHeight, position.y + scale.y); }
		}
		//OBB
		if (fabs(obbX) < scale.x && fabs(obbZ) < scale.z && fabs(obbY) < scale.y)
		{
			//�ǂꂾ�������o�����𒲂ׂ�
			D3DXVECTOR3 penetration = D3DXVECTOR3(static_cast<float>(scale.x - fabs(obbX)), static_cast<float>(scale.y - fabs(obbY)), static_cast<float>(scale.z - fabs(obbZ)));

			//���E����G���
			if (penetration.x < penetration.z && penetration.x < penetration.y)
			{
				m_Position += (obbX > 0) ? (penetration.x * right) : (-penetration.x * right);
			}
			//�O�ォ��G���
			else if (penetration.z < penetration.y)
			{
				m_Position += (obbZ > 0) ? (penetration.z * forward) : (-penetration.z * forward);
			}
			//�㉺����G���
			else
			{
				m_Position += (m_Position.y < obbY) ? (-penetration.y * up) : penetration.y * up;
				m_Velocity.y = (m_Position.y < obbY) ? -m_Velocity.y : 0.0f; // ��ɏ�����琂�����x��0�ɂ���A������G�ꂽ�甽�]����
			}
		}
	}

	if (m_Position.y < 0.0f && m_Velocity.y < 0.0f)
	{
		m_Position.y = 0.0f;
		m_Velocity.y = 0.0f;
	}
}
