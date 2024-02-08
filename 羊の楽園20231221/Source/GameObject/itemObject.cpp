#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\itemObject.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\follow.h"
#include "..\Scene\scene.h"
#include "..\GameObject\camera.h"

#define INITIAL_VELOCITY D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INITIAL_ORIGINAL_SCALE D3DXVECTOR3(1.0f, 1.0f, 1.0f)
#define INITIAL_DAATH_TIME 500
#define INITIAL_DELETE_INIT false
#define INITIAL_DROP_JUMP false
#define INITIAL_SCALE_Y 0.01f
#define GRAVITY 0.015f

void ItemObject::Init()
{
	m_Scale.y = INITIAL_SCALE_Y;

	m_Velocity = INITIAL_VELOCITY;
	m_OriginalScale = INITIAL_ORIGINAL_SCALE;
	m_DaathTime = INITIAL_DAATH_TIME;
	m_DeleteInit = INITIAL_DELETE_INIT;
	m_DropJump = INITIAL_DROP_JUMP;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\pixelLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\pixelLightingPS.cso");

	//�A�^�b�`
	m_Shadow = AddComponent<Shadow>();
	m_Shadow->SetScale(D3DXVECTOR3(0.4f, 0.4f, 0.4f));
}

void ItemObject::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ItemObject::Update()
{
	GameObject::Update();

	switch (m_ItemState)
	{
	case ITEM_STATE::NORMAL:
		UpdateMove();
		break;
	case ITEM_STATE::DEATH:
		UpdateDelete();
		break;
	}

	//������]
	m_Rotation.y += 0.05f;

	//�d��
	float groundHeight = 0.0f;	//�n�ʂ̍���
	m_Velocity.y -= GRAVITY;		//������

	//�ڒn
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity *= 0.0f;
		m_DropJump = false;
	}

	//�ړ�
	m_Position += m_Velocity;

	//�e�̈ړ�
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y += 0.02f;
	m_Shadow->SetPosition(shadowPosition);
}

void ItemObject::UpdateMove()
{
	Scene* scene = Manager::GetScene();
	
	//�������ł���܂ł̎���
	m_DaathTime--;
	if (m_DaathTime <= 0) { m_ItemState = ITEM_STATE::DEATH; }

	//�ʂ�ʂ�o��
	SmoothAppearance(true);

	//�W�����v���Ă��炱�̌�̏��������Ȃ�
	if (m_DropJump) { return; }
	
	//�f�[�^�擾
	Player* player = scene->GetGameObject<Player>();
	auto follows = scene->GetGameObjects<Follow>();

	//�v���C���[�̋������擾
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	D3DXVECTOR3 scale = player->GetScale();
	float length = D3DXVec3Length(&direction);
	if (length < scale.x)
	{
		if (!m_DeleteInit)
		{
			MoveGet();
			m_DeleteInit = true;
		}
		 m_ItemState = ITEM_STATE::DEATH;
	}

	for (Follow* follow : follows) {
		if (follow->GetState() != FOLLOW_STATE::FREE) {
			D3DXVECTOR3 position = follow->GetPosition();
			D3DXVECTOR3 scale = follow->GetScale();
			D3DXVECTOR3 direction = m_Position - position;
			float length = D3DXVec3Length(&direction);

			if (length < scale.x)
			{
				if (!m_DeleteInit)
				{
					MoveGet();
					m_DeleteInit = true;
				}
				m_ItemState = ITEM_STATE::DEATH;
			}
		}
	}
}

void ItemObject::UpdateDelete()
{
	//�ʂ�ʂ����
	m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SmoothAppearance(false);
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void ItemObject::SmoothAppearance(bool growing)
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

void ItemObject::SetDrop()
{
	m_Velocity.y = 0.25f;
	m_Rotation.y = frand() * 2 * D3DX_PI;
	m_Velocity.x = GetForward().x * (frand()* 0.08f);
	m_Velocity.z = GetForward().z * (frand()* 0.08f);
	m_DropJump = true;
}
