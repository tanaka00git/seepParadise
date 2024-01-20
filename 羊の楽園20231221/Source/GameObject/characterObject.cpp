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
		UpdateDeath();
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

	//�~��
	auto cylinders = scene->GetGameObjects<Cylinder>();		//�~���̃��X�g���擾
	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		scale.x *= 1.2f, scale.z *= 1.2f;					//���a�̒���
		D3DXVECTOR3 direction = m_Position - position;		//�~�����S����L�����܂ł̃x�N�g��
		float length = D3DXVec3Length(&direction);			//�L�����Ɖ~�����S�̋���
		D3DXVECTOR3 up = cylinder->GetUp();					//�~���̏�����x�N�g��
		float obbr = D3DXVec3Dot(&direction, &up);			//�~���̏�����ւ̎�����

		direction -= up * obbr;	//y���̐ڐG�𖳎����邽�߂ɁAdirection����up�̐���������
		length = D3DXVec3Length(&direction); //�V����length���v�Z����

		//�e
		if (length < scale.x && m_Position.y > position.y + scale.y - 0.5f)
		{
			groundHeight = max(groundHeight, position.y + scale.y);
		}
		// OBB
		if (length < scale.x && fabs(obbr) < scale.y)
		{
			// �~���̏㉺�����ւ̉����o������
			float penetrationH = scale.y - static_cast<float>(fabs(obbr));
			float penetrationX = scale.x - length;

			// ������G�ꂽ�ꍇ�̏���
			if (penetrationX < penetrationH)
			{
				// �~�����ɓ������ꍇ�̏���
				D3DXVECTOR3 normalizedDirection = direction / length;
				float penetration = scale.x - length;

				// �����������ɉ����o������
				m_Position += penetration * normalizedDirection;
			}
			else
			{
				if (obbr > 0)
				{
					m_Position += penetrationH * up;
					m_Velocity.y = 0.0f;  // ��ɏ�����琂�����x��0�ɂ���
				}
				else
				{
					m_Position -= penetrationH * up;
					m_Velocity.y -= m_Velocity.y; // ������G�ꂽ�琂�����x�𔽓]����
				}
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
		float obbx = D3DXVec3Dot(&direction, &right);	//X�����������L��������
		float obbz = D3DXVec3Dot(&direction, &forward);	//Z�����������L��������
		float obby = D3DXVec3Dot(&direction, &up);		//Y�����������L��������

		//�e�̍����ݒ�
		if (fabs(obbx) < scale.x && fabs(obbz) < scale.z)
		{
			if (m_Position.y > position.y + scale.y - 0.5f) { groundHeight = max(groundHeight, position.y + scale.y); }
		}
		//OBB
		if (fabs(obbx) < scale.x && fabs(obbz) < scale.z && fabs(obby) < scale.y)
		{
			//�L�����������̂�����ɂ��邩�ǂ����̔���
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
					m_Velocity.y = 0.0f;			//��ɏ�����琂�����x��0�ɂ���
				}
				else
				{
					m_Position -= penetration.y * up;
					m_Velocity.y = -m_Velocity.y;	//������G�ꂽ�琂�����x�𔽓]����
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
