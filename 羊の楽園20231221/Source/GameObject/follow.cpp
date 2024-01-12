#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\App\audio.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\box.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\smoke.h"
#include "..\App\model.h"

Model*Follow::m_Model{};
Audio*Follow::m_SE_Follow{};
Audio*Follow::m_SE_Release{};
bool Follow::m_SE_FollowCheck{};

#define WALK_EFFECT_TIME 10		//�G�t�F�N�g���\�������Ԋu�t���[��
#define CONTACT_EXTRUSION 0.02f	//���̒��ԂƐڐG�����ۂ̉����o���W��
#define MOVE_SPEED_NORMAL 0.01f	//�ړ����x
#define MOVE_SPEED_DASH   0.02f //�_�b�V�����ړ����x
#define MOVE_MAGNIFY_FREE 6.5f  //���R��Ԃ̈ړ����x�̔{��(����ȊO�̏�Ԃł̓v���C���[�Ɠ���)
#define DELETE_DISTANCE   25.0f //�v���C���[�Ɨ���Ă���Ƃ��Ɏ������ł��鋗��
#define INIT_JUMP 0.25f			//�o���̃W�����v�̍���

void Follow::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\seep_c.obj");
	m_SE_Follow = new Audio();
	m_SE_Follow->Load("asset\\audio\\���킢�����˂�E�W�����v03.wav");
	m_SE_Release = new Audio();
	m_SE_Release->Load("asset\\audio\\�����E�t�H�V���b.wav");
}

void Follow::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Follow::Init()
{
	m_Scale.y = 0.01f;
	m_Rotation.y = frand() * 2 * D3DX_PI;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_Shadow = AddComponent<Shadow>();
}

void Follow::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Follow::Update()
{
	GameObject::Update();

	//���S���̏���
	if (m_Life <= 0) { m_FollowState = FOLLOW_STATE::DEATH; }

	switch (m_FollowState)
	{
	case FOLLOW_STATE::FREE:
		UpdateFree();
		break;
	case FOLLOW_STATE::NORMAL:
		UpdateNormal();
		break;
	case FOLLOW_STATE::DASH:
		UpdateDash();
		break;
	case FOLLOW_STATE::DEATH:
		UpdateDeath();
		break;
	}

	//�U����~(�܂��U���ł���܂�)
	AttackStop();

	//�d��
	m_Velocity.y -= 0.015f;

	//��Q���Ƃ̏Փ˔���
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//�ړ�
	m_Position += m_Velocity;

	//�ڒn
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	//�e�̈ړ�
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
	m_Shadow->SetScale(D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z));

	//�^���A�j��
	Anime();
	m_SE_FollowCheck = false;
}

void Follow::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera->CheckView(m_Position, m_Scale)) { return; }

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

	m_Model->Draw();
}

void Follow::UpdateFree()
{
	//�ʂ�ʂ�o��
	if (m_Scale.y < 1.0f) { m_Scale.y += 0.05f; }
	
	//�Ē��ԉ��X�p��
	m_AgainFollow --;
	if (m_AgainFollow <= 0) { m_AgainFollow = 0; }

	//�ړ����x�ݒ�
	m_Speed = MOVE_MAGNIFY_FREE;

	//�����ύX
	OrtOrientationChange();

	//�ړ�
	m_Velocity.x = GetForward().x * MOVE_SPEED_NORMAL;
	m_Velocity.z = GetForward().z * MOVE_SPEED_NORMAL;

	//�ߊ�����璇�ԉ�
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVECTOR3 scale = player->GetScale();
	
	if (length < scale.x && length < scale.y && m_AgainFollow <= 0)
	{ 
		m_SE_Follow->Play(1.0f); 
		m_FollowState = FOLLOW_STATE::NORMAL;
	}

	//�v���C���[���炩�Ȃ藣��Ă�����폜
	if (length > DELETE_DISTANCE) { SetDestroy(); }
}

void Follow::UpdateNormal()
{
	//�ʂ�ʂ�o��
	if (m_Scale.y < 1.0f) { m_Scale.y += 0.05f; }

	m_Velocity.x = GetForward().x * (m_Speed * MOVE_SPEED_NORMAL);
	m_Velocity.z = GetForward().z * (m_Speed * MOVE_SPEED_NORMAL);

	PlayerTracking();
}

void Follow::UpdateDash()
{
	//�ʂ�ʂ�o��
	if (m_Scale.y < 1.0f) { m_Scale.y += 0.05f; }

	//�G�t�F�N�g
	WalkEffect();

	//�ړ�
	m_Velocity.x = GetForward().x * (m_Speed * MOVE_SPEED_DASH);
	m_Velocity.z = GetForward().z * (m_Speed * MOVE_SPEED_DASH);
	
	PlayerTracking();
}

void Follow::UpdateDeath()
{
	m_Velocity *= 0;

	m_Rotation.z += m_Death / 2.0f;
	m_Rotation.y -= m_Death;
	m_Scale.x -= 0.01f; m_Scale.y -= 0.01f; m_Scale.z -= 0.01f;
	m_Death -= 0.001f;

	if (m_Rotation.z > 3.1415f / 4) { m_Rotation.z = 3.1415f / 4; }
	if (m_Death < 0.0f) { m_Death = 0.0f; }
	if (m_Scale.y < 0.0f)
	{
		m_Scale *= 0.0f;
		SetDestroy();
	}
}

void Follow::Collision(float & groundHeight)
{
	Scene* scene = Manager::GetScene();
	auto follows = scene->GetGameObjects<Follow>();

	//��Ǘr�Ƃ̊֌W��
	for (Follow * follow : follows)
	{
		//�������g�̏ꍇ�̓X�L�b�v
		if (follow == this) { continue; }
		
		D3DXVECTOR3 position = follow->GetPosition();
		D3DXVECTOR3 scale = follow->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float flength = D3DXVec3Length(&direction);

		if (flength < scale.x && flength < scale.y)
		{
			//���̗r�Ƃ̐ڐG�ŃY����
				m_Velocity.x += (m_Position.x - follow->m_Position.x) * CONTACT_EXTRUSION;
				m_Velocity.z += (m_Position.z - follow->m_Position.z) * CONTACT_EXTRUSION;

			//���ԗr�ɐG�ꂽ�璇�Ԃ�
			if (follow->GetState() == FOLLOW_STATE::FREE && m_FollowState != FOLLOW_STATE::FREE && m_AgainFollow <= 0)
			{
				if (!m_SE_FollowCheck)
				{
					m_SE_Follow->Play(1.0f);
					m_SE_FollowCheck = true;
				}
				follow->SetState(FOLLOW_STATE::NORMAL);
			}
		}
	}

	//���b�N
	auto rocks = scene->GetGameObjects<Rock>();
	for (Rock* rock : rocks) 
	{
		if (rock->GetState() != BREAKOBJECT_STATE::DEATH)
		{
			D3DXVECTOR3 position = rock->GetPosition();
			D3DXVECTOR3 scale = rock->GetScale();
			if (position.x - scale.x < m_Position.x && m_Position.x < position.x + scale.x &&
				position.z - scale.z < m_Position.z && m_Position.z < position.z + scale.z) {
				if (m_Position.y < position.y + scale.y) {//2.0f�̓��f���Œ���
					m_Velocity.x = (m_Position.x - position.x) * CONTACT_EXTRUSION;
					m_Velocity.z = (m_Position.z - position.z) * CONTACT_EXTRUSION;
				}
				else { groundHeight = position.y + scale.y; }	//�������2.0
			}
		}
	}
	//�`�F�X�g
	auto chests = scene->GetGameObjects<Chest>();
	for (Chest* chest : chests)
	{
		if (chest->GetState() != BREAKOBJECT_STATE::DEATH)
		{
			D3DXVECTOR3 position = chest->GetPosition();
			D3DXVECTOR3 scale = chest->GetScale();
			if (position.x - scale.x < m_Position.x && m_Position.x < position.x + scale.x &&
				position.z - scale.z < m_Position.z && m_Position.z < position.z + scale.z) {
				if (m_Position.y < position.y + scale.y) {//2.0f�̓��f���Œ���
					m_Velocity.x = (m_Position.x - position.x) * CONTACT_EXTRUSION;
					m_Velocity.z = (m_Position.z - position.z) * CONTACT_EXTRUSION;
				}
				else { groundHeight = position.y + scale.y; }	//�������2.0
			}
		}
	}
	//�~�n
	auto cylinders = scene->GetGameObjects<Cylinder>();
	for (Cylinder * cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);
		if (length < scale.x*1.2f) {//*1.2�͒���
			if (m_Position.y < position.y + scale.y - 0.5f) {
				m_Velocity.x = (m_Position.x - position.x) * CONTACT_EXTRUSION;
				m_Velocity.z = (m_Position.z - position.z) * CONTACT_EXTRUSION;

			}
			else { groundHeight = position.y + scale.y; }
		}
	}

	//������
	auto boxs = scene->GetGameObjects<Box>();
	for (Box* box : boxs)
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();

		if (position.x - scale.x < m_Position.x && m_Position.x < position.x + scale.x &&
			position.z - scale.z < m_Position.z && m_Position.z < position.z + scale.z) {
			if (m_Position.y < position.y + scale.y * 1.8f - 0.5f) {//2.0f�̓��f���Œ���
				m_Velocity.x = (m_Position.x - position.x) * CONTACT_EXTRUSION;
				m_Velocity.z = (m_Position.z - position.z) * CONTACT_EXTRUSION;

			}
			else { groundHeight = position.y + scale.y * 1.8f; }
		}
	}
}

void Follow::Anime()
{
	m_AnimeTime++;
	
	//���Ԓ���
	int time = 14;
	if (m_FollowState == FOLLOW_STATE::DASH) { time = 4; }

	if (m_AnimeTime > time)
	{
		//�X����
		if (m_FollowState == FOLLOW_STATE::DASH) { m_Rotation.x = (0.08f * m_AnimePause); }
		else { m_Rotation.x = (0.03f * m_AnimePause); }

		//���Z�b�g
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
	}
}

void Follow::DamageFlash()
{
	if (m_DamageFlashTime > 0)
	{
		m_DamageFlashTime--;
		m_TextureEnable = false;
	}
	else
	{
		m_TextureEnable = true;
	}
}

void Follow::OrtOrientationChange()
{
	m_OrientationTime ++;
	//�����ύX
	if (m_OrientationTime > m_NextOrientationTime){ m_Rotation.y += 0.02f * m_NextRot;}
	if (m_OrientationTime > m_NextOrientationTime + 50)
	{
		//����̌�����ݒ�
		int a = rand() * 2;
		if (a == 1) { m_NextRot *= -1; }

		// ���̉�]�܂ł̎��Ԃ������_���ɐݒ�
		m_NextOrientationTime = irand(80, 200);

		// ���ԃ��Z�b�g
		m_OrientationTime = 0;
	}

}

void Follow::WalkEffect()
{
	m_WalkEffectTime++;

	//�G�t�F�N�g�����ʒu����(�L�����̌��)
	D3DXVECTOR3 effectPosition = m_Position;
	effectPosition.x -= GetForward().x * 0.8f;
	effectPosition.z -= GetForward().z * 0.8f;

	//���t���[����������G�t�F�N�g����
	if (m_WalkEffectTime >= WALK_EFFECT_TIME)
	{
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Smoke>(1)->SetPosition(effectPosition);//�����G�t�F�N�g
		m_WalkEffectTime = 0;
	}
}

void Follow::PlayerTracking()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	float length = D3DXVec3Length(&direction);
	D3DXVECTOR3 scale = player->GetScale();

	//�v���C���[�͕�
	PLAYER_STATE playerstate = player->GetState();
	if (playerstate == PLAYER_STATE::NORMAL)
	{
		m_FollowState = FOLLOW_STATE::NORMAL;
	}
	else if (playerstate == PLAYER_STATE::DASH)
	{
		m_FollowState = FOLLOW_STATE::DASH;
	}


	m_Rotation.y = player->GetRotation().y;
	m_Speed = player->GetSpeed();

	//�v���C���[�ɒn���ɋߕt��
	m_Velocity.x += (player->GetPosition().x - m_Position.x)*((length * 3) *0.0005f);
	m_Velocity.z += (player->GetPosition().z - m_Position.z)*((length * 3) *0.0005f);

	//�v���C���[�ɐڐG���Ă���Y����
	if (length < scale.x)
	{
		m_Velocity.x += (m_Position.x - player->GetPosition().x) * CONTACT_EXTRUSION;
		m_Velocity.z += (m_Position.z - player->GetPosition().z) * CONTACT_EXTRUSION;
	}
	//�v���C���[���痣�ꂷ���Ă��璇�ԉ���
	if (length > 8.0f)
	{
		m_AgainFollow = 20;
		m_SE_Release->Play(1.0f);
		m_FollowState = FOLLOW_STATE::FREE;
	}
}

void Follow::AttackStop()
{
	m_AttackStopTime --;
	if (m_AttackStopTime > 0)
	{
		m_Velocity.x = (GetForward().x * 0.03f) * -1;
		m_Velocity.z = (GetForward().z * 0.03f) * -1;
	}
	if (m_AttackStopTime <= 0) { m_AttackStopTime = 0; }
}

void Follow::SetDrop()
{
	m_Velocity.y = INIT_JUMP;
	m_Rotation.y = frand() * 90;
	m_Velocity.x = GetForward().x * (frand() * 0.08f);
	m_Velocity.z = GetForward().z * (frand() * 0.08f);
}