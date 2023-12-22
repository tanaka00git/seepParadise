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

void ItemObject::Init()
{
	m_Scale.y = 0.01f;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

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
	m_Velocity.y -= 0.015f;		//������

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
	shadowPosition.y += 0.01f;
	m_Shadow->SetPosition(shadowPosition);
}

void ItemObject::Draw()
{
	GameObject::Draw();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);
}

void ItemObject::UpdateMove()
{
	Scene* scene = Manager::GetScene();
	
	//�������ł���܂ł̎���
	m_DaathTime--;
	if (m_DaathTime <= 0) { m_ItemState = ITEM_STATE::DEATH; }

	//�ʂ�ʂ�o��
	if (m_Scale.y < 1.0f) { m_Scale.y += 0.05f; }
	else { m_Scale.y = 1.0f; }

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
			direction.y = 0.0f;
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
	m_Scale.x -= 0.1f; m_Scale.y -= 0.1f; m_Scale.z -= 0.1f;
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void ItemObject::SetDrop()
{
	m_Velocity.y = 0.25f;
	m_Rotation.y = frand() * 90;
	m_Velocity.x = GetForward().x * (frand()* 0.08f);
	m_Velocity.z = GetForward().z * (frand()* 0.08f);
	m_DropJump = true;
}
