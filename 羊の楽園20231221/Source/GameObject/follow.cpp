#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\App\audio.h"
#include "..\GameObject\score.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\smoke.h"
#include "..\GameObject\angelRing.h"
#include "..\App\model.h"

Model*Follow::m_Model{};
Audio*Follow::m_SE_Follow{};
Audio*Follow::m_SE_Release{};
bool Follow::m_SE_FollowCheck{};

#define WALK_EFFECT_TIME 13		//�G�t�F�N�g���\�������Ԋu�t���[��
#define CONTACT_EXTRUSION 0.02f	//���̒��ԂƐڐG�����ۂ̉����o���W��
#define MOVE_SPEED_NORMAL 0.01f	//�ړ����x
#define MOVE_SPEED_DASH   0.02f //�_�b�V�����ړ����x
#define MOVE_MAGNIFY_FREE 6.5f  //���R��Ԃ̈ړ����x�̔{��(����ȊO�̏�Ԃł̓v���C���[�Ɠ���)
#define DELETE_DISTANCE   25.0f //�v���C���[�Ɨ���Ă���Ƃ��Ɏ������ł��鋗��
#define ATTACK_STOP 23
#define GRAVITY 0.015f

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
	CharacterObject::Init();

	m_Scale.y = 0.01f;
	m_Rotation.y = frand() * 2 * D3DX_PI;
	m_WalkEffectTime = irand(0, WALK_EFFECT_TIME);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_Shadow = AddComponent<Shadow>();
}

void Follow::Uninit()
{
	CharacterObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Follow::Update()
{
	CharacterObject::Update();

	//���S���̏���
	if (m_Life <= 0) { m_CharacterState = CHARACTER_STATE::DEAD; }

	//�U����~(�܂��U���ł���܂�)
	AttackStop();

	//�d��
	m_Velocity.y -= GRAVITY;

	//�����蔻��
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
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x + m_AnimeRotationX, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

void Follow::UpdateAlive()
{
	CharacterObject::UpdateAlive();

	//�ʂ�ʂ�o��
	if (m_Scale.y < m_OriginalScale.y) { m_Scale.y += 0.05f; }

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
	}
}

void Follow::UpdateFree()
{
	
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
	
	if (length < scale.x && length < scale.y && length < scale.z && m_AgainFollow <= 0)
	{ 
		m_SE_Follow->Play(1.0f); 
		m_FollowState = FOLLOW_STATE::NORMAL;
	}

	//�v���C���[���炩�Ȃ藣��Ă�����폜
	if (length > DELETE_DISTANCE) { SetDestroy(); }
}

void Follow::UpdateNormal()
{
	m_Velocity.x = GetForward().x * (m_Speed * MOVE_SPEED_NORMAL);
	m_Velocity.z = GetForward().z * (m_Speed * MOVE_SPEED_NORMAL);

	PlayerTracking();
}

void Follow::UpdateDash()
{
	//�G�t�F�N�g
	WalkEffect();

	//�ړ�
	m_Velocity.x = GetForward().x * (m_Speed * MOVE_SPEED_DASH);
	m_Velocity.z = GetForward().z * (m_Speed * MOVE_SPEED_DASH);
	
	PlayerTracking();
}

void Follow::UpdateDead()
{
	m_Velocity *= 0;

	m_Rotation.z += m_Death / 2.0f;
	m_Rotation.y -= m_Death;
	m_Scale.x -= 0.008f; m_Scale.y -= 0.008f; m_Scale.z -= 0.008f;
	m_Death -= 0.001f;

	if (m_Rotation.z > 3.1415f / 4) { m_Rotation.z = 3.1415f / 4; }
	if (m_Death < 0.0f) { m_Death = 0.0f; }
	if (m_Scale.y < 0.0f)
	{
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<AngelRing>(1)->SetPosition(m_Position);//�V�g�G�t�F�N�g
		m_WalkEffectTime = 0;

		m_Scale *= 0.0f;
		SetDestroy();
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
		if (m_FollowState == FOLLOW_STATE::DASH) { m_AnimeRotationX = (0.08f * m_AnimePause); }
		else { m_AnimeRotationX = (0.03f * m_AnimePause); }

		//���Z�b�g
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
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

	//���Ԃ̗ʂŌ��炷
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();
	int count = score->GetCount();

	//���t���[����������G�t�F�N�g����
	if (m_WalkEffectTime >= WALK_EFFECT_TIME + count / 5)
	{
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Smoke>(1)->SetPosition(effectPosition);//���G�t�F�N�g
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
	PLAYER_STATE aliveState = player->GetPlayerState();
	if (aliveState == PLAYER_STATE::NORMAL)
	{
		m_FollowState = FOLLOW_STATE::NORMAL;
	}
	else if (aliveState == PLAYER_STATE::DASH)
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

void Follow::Collision(float& groundHeight)
{
	CharacterObject::Collision(groundHeight);

	Scene* scene = Manager::GetScene();
	//��Ǘr�Ƃ̊֌W��
	auto follows = scene->GetGameObjects<Follow>();
	for (Follow* follow : follows)
	{
		//�������g�̏ꍇ�̓X�L�b�v
		if (follow == this) { continue; }

		D3DXVECTOR3 position = follow->GetPosition();
		D3DXVECTOR3 scale = follow->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		float length = D3DXVec3Length(&direction);

		if (length < scale.x && length < scale.y && length < scale.z)
		{
			//���̗r�Ƃ̐ڐG�ŃY����
			m_Velocity.x += (m_Position.x - follow->m_Position.x) * CONTACT_EXTRUSION;
			m_Velocity.z += (m_Position.z - follow->m_Position.z) * CONTACT_EXTRUSION;

			//���������ԂłȂ��Ƃ��ɒ��ԗr�ɐG�ꂽ�璇�Ԃ�
			if (follow->GetState() != FOLLOW_STATE::FREE && m_FollowState == FOLLOW_STATE::FREE && m_AgainFollow <= 0)
			{
				if (!m_SE_FollowCheck)
				{
					m_SE_Follow->Play(1.0f);
					m_SE_FollowCheck = true;
				}
				m_FollowState = FOLLOW_STATE::NORMAL;
			}
		}
	}

	//�j��u���b�N
	auto breakObjects = scene->GetGameObjects<BreakObject>();
	for (BreakObject* breakObject : breakObjects)
	{
		if (breakObject->GetState() != BREAKOBJECT_STATE::DEATH)
		{
			D3DXVECTOR3 position = breakObject->GetPosition();
			D3DXVECTOR3 scale = breakObject->GetScale();
			D3DXVECTOR3 right = breakObject->GetRight();		//x������
			D3DXVECTOR3 forward = breakObject->GetForward();	//z������
			D3DXVECTOR3 up = breakObject->GetUp();				//y������
			D3DXVECTOR3 direction = m_Position - position;		//�����̂���L�����܂ł̕����x�N�g��
			float obbX = D3DXVec3Dot(&direction, &right);
			float obbZ = D3DXVec3Dot(&direction, &forward);
			float obbY = D3DXVec3Dot(&direction, &up);

			// OBB
			if (fabs(obbX) < scale.x && fabs(obbZ) < scale.z && fabs(obbY) < scale.y)
			{
				// �_�b�V���U��
				if (m_FollowState == FOLLOW_STATE::DASH && GetAttackStop() <= 0)
				{
					breakObject->SetDamageMove();
					SetAttackStop(ATTACK_STOP);
					scene->GetGameObject<Player>()->AddCombo(1);
				}

				D3DXVECTOR3 penetration = D3DXVECTOR3(fabsf(scale.x) - fabsf(obbX), fabsf(scale.y) - fabsf(obbY), fabsf(scale.z) - fabsf(obbZ));

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
	}
}