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

// �萔
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
	m_SE_Kick->Load("asset\\audio\\���L�b�Nb.wav");
}

void BreakObject::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
	
	//������
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

	//�A�^�b�`
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

	//�����蔻��
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//�_���[�W�t���b�V��
	DamageFlash();
}

void BreakObject::UpdateMove()
{
	Scene* scene = Manager::GetScene();

	//�ʂ�ʂ�o��
	SmoothAppearance(true);

	//HP�o�[�\��
	LifeBarMove();

	//�d��
	m_Velocity.y -= GRAVITY;

	//�ړ�
	m_Position += m_Velocity;	

	//��Q���Ƃ̏Փ˔���
	float groundHeight = 0.0f;

	//�ڒn
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	//���C�t���Ȃ��Ȃ�����
	if (m_Life <= 0)
	{
		m_Life = 0;
		m_BreakObjectState = BREAKOBJECT_STATE::DEATH;
	}
}

void BreakObject::UpdateDelete()
{
	//�ʂ�ʂ����
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
	//�X�P�[���ݒ�
	//HP���ő�HP�ƈꏏ����Ȃ���΃��C�t�o�[�\��
	if (m_FullLife != m_Life) { m_HpBarS->SetDraw(true); }

	//HP�o�[�̈ʒu�ύX
	D3DXVECTOR3 HpBarPosition = m_Position;
	HpBarPosition.y += m_HpBarPosY;
	m_HpBarS->SetPosition(HpBarPosition);

	//��񑗐M
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
}

void BreakObject::Collision(float& groundHeight)
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
				m_Position += (m_Position.y < obbY) ? (-penetrationY * up) : penetrationY * up;
				m_Velocity.y = (m_Position.y < obbY) ? -m_Velocity.y : 0.0f; // ��ɏ�����琂�����x��0�ɂ���A������G�ꂽ�甽�]����
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
