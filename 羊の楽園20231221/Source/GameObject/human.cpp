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
Audio* Human::m_SE_Make{};

#define LIFE 4
#define SPEED 0.08f
#define MAKING_TIME 30
#define MAKING_COUNT 4
#define DROP_RATE 20
#define APPLE_RATE 20
#define GIVE_ATTACK_STOP 20
#define KNOCK_BACK_TIME 14
#define STUN_TIME 60
#define TARGET_LENGTH 3.0f

void Human::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\human.obj");
	m_SE_Kick = new Audio();
	m_SE_Kick->Load("asset\\audio\\���L�b�Nb.wav");
	m_SE_Make = new Audio();
	m_SE_Make->Load("asset\\audio\\�؍ނɓB��ł�.wav");
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

	//�����폜�܂�
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

	//�d��
	m_Velocity.y -= 0.015f;

	//�m�b�N�o�b�N
	KnockBack();

	//��Q���Ƃ̏Փ˔���
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//�ړ�
	m_Position += m_Velocity;

	//HP���ő�HP�ƈꏏ����Ȃ���΃��C�t�o�[�\��
	if (m_FullLife != m_Life) { m_HpBarS->SetScale(m_BarScale); }
	else { m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)); }

	//�e�̈ړ�
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
	m_Shadow->SetScale(D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z));

	//HP�o�[�\��
	D3DXVECTOR3 HpBarPosition = m_Position;
	HpBarPosition.y += m_HpBarPosY;
	m_HpBarS->SetPosition(HpBarPosition);

	//�ڒn
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	//�^���A�j��
	Anime();

	//�_���[�W�t���b�V��
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

	// �}�g���N�X�ݒ�
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
	//�����ύX
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

	//�ړ�
	m_Velocity.x = GetForward().x * m_Speed / 2;
	m_Velocity.z = GetForward().z * m_Speed / 2;

	//�v���C���[���͈͓��ł����
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	float plength = D3DXVec3Length(&direction);

	//�ǔ��͈͂ɓ��邩��ł����
	TimeFade* timeFade = scene->GetGameObject<TimeFade>();
	if (plength < m_Tracking) { m_HumanState = HUMAN_STATE::TARGETING; }
	if (timeFade->GetTimeZone()) { m_HumanState = HUMAN_STATE::NIGHT; }
}

void Human::UpdateTargeting()
{
	Scene* scene = Manager::GetScene();
	TimeFade* timeFade = scene->GetGameObject<TimeFade>();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	float plength = D3DXVec3Length(&direction);

	//���ԗr������ΗD��I��
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
			//���܂ł�for���̒��ň�ԋ߂����W�ɂ����Ȃ�������X�V
			if (length < nearestDistance)
			{
				nearestDistance = length;
				nearestFollow = follow;
			}
		}
	}

	//��ԋ߂��t�H���[�I�u�W�F�N�g����������������ύX
	if (nearestFollow != nullptr)
	{
		m_Rotation.y = atan2f(nearestFollow->GetPosition().x - m_Position.x, nearestFollow->GetPosition().z - m_Position.z);
	}
	//�łȂ���΃v���C���[�̕��Ɍ����ύX
	else { m_Rotation.y = atan2f((player->GetPosition().x) - m_Position.x, (player->GetPosition().z) - m_Position.z); }

	//�ړ�����(����)
	bool timeFede = timeFade->GetTimeZone();
	m_Velocity.x = GetForward().x * m_Speed;
	m_Velocity.z = GetForward().z * m_Speed;

	//�����͈͂𒴂�����
	if (plength > m_Tracking) { m_HumanState = HUMAN_STATE::FREE; }//�ǔ����Ȃ�
	//��Ȃ�
	if (timeFede) { m_HumanState = HUMAN_STATE::NIGHT; }
}

void Human::UpdateDelete()
{
	//�ʂ�ʂ����
	m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Scale.x -= 0.05f; m_Scale.y -= 0.05f; m_Scale.z -= 0.05f;
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void Human::UpdateMaking()
{
	//�ړ����Ȃ�
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_MakingTime++;
	if (m_MakingTime % MAKING_TIME == 0)
	{
		//�I�u�W�F�N�g�����ʒu����(���ז��҂̑O)
		D3DXVECTOR3 objectPosition = m_Position;
		objectPosition.x += GetForward().x;
		objectPosition.z += GetForward().z;
		
		//���ʉ��ƃG�t�F�N�g
		m_SE_Make->Play(1.0f);
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Explosion>(1)->SetPosition(objectPosition);//�����G�t�F�N�g


		if (m_MakingTime >= MAKING_TIME * MAKING_COUNT)
		{
			m_MakingTime = 0;
			m_HumanState = HUMAN_STATE::FREE;

			//�򔭐�
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
		UpdateMaking();
		break;
	case HUMAN_STATE::DAMAGE:
		UpdateDamage();
		break;
	case HUMAN_STATE::NIGHT:
		UpdateNight();
		break;
	case HUMAN_STATE::TARGETING:
		UpdateTargeting();
		break;
	}
}

void Human::UpdateDead()
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

		//�����S�h���b�v
		if (m_Item)
		{
			Apple* apple = scene->AddGameObject<Apple>(1);
			apple->SetPosition(m_Position);
			apple->SetDrop();
		}

		//�A�C�e���h���b�v
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
		scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g
		SetDestroy();
	}

}

void Human::UpdateDamage()
{
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	Escape();

	//�U���󂯂��Ƃ��̏���
	m_StunTime--;
	if (m_StunTime <= 0)
	{
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_HumanState = HUMAN_STATE::FREE;
	}
}

void Human::UpdateNight()
{
	Escape();

	Scene* scene = Manager::GetScene();
	TimeFade* timeFade = scene->GetGameObject<TimeFade>();
	if (!timeFade->GetTimeZone()) { m_HumanState = HUMAN_STATE::FREE; }
}

void Human::KnockBack()
{
	//���
	if (m_KnockBackTime >= 1)
	{
		m_KnockBackTime--;
		m_Velocity.x += (m_PLForward.x * (m_KnockBackTime * 0.02f));
		m_Velocity.z += (m_PLForward.z * (m_KnockBackTime * 0.02f));
		if (m_KnockBackTime <= 0) { m_KnockBackTime = 0; }
	}
}

void Human::Escape()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//�v���C���[�̔��΂�����
	m_Rotation.y = atan2f(m_Position.x - player->GetPosition().x, m_Position.z - player->GetPosition().z);

	//�ړ�����(����)
	m_Velocity.x = GetForward().x * (m_Speed * 2.0f);
	m_Velocity.z = GetForward().z * (m_Speed * 2.0f);
}

void Human::Anime()
{
	m_AnimeTime++;
	if (m_AnimeTime > 7)
	{
		//�X����
		if (m_HumanState == HUMAN_STATE::TARGETING || m_HumanState == HUMAN_STATE::MAKING) { m_AnimeRotationX = (0.16f * m_AnimePause); }
		else { m_AnimeRotationX = (0.04f * m_AnimePause); }

		//���Z�b�g
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
	m_MakingTime = 0;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	m_PLForward = player->GetForward();
	scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g

	Camera* camera = scene->GetGameObject<Camera>();
	camera->SetShake(0.1f, 0.0f);
}

void Human::Collision(float& groundHeight)
{
	if (m_CharacterState == CHARACTER_STATE::DEAD) { return; };

	CharacterObject::Collision(groundHeight);
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//�L�����N�^�[�I�u�W�F�N�g�Ƃ̐ڐG����
	auto characterObjects = scene->GetGameObjects<CharacterObject>();
	for (CharacterObject* characterObject : characterObjects)
	{
		//�������g�̏ꍇ�̓X�L�b�v
		if (characterObject == this) { continue; }

		D3DXVECTOR3 position = characterObject->GetPosition();
		D3DXVECTOR3 scale = characterObject->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		float length = D3DXVec3Length(&direction);

		//�ǂ̃I�u�W�F�N�g������
		Player* playerObject = dynamic_cast<Player*>(characterObject);
		Follow* followObject = dynamic_cast<Follow*>(characterObject);

		if (length < scale.x && length < scale.y && length < scale.z)
		{
			//�I�u�W�F�N�g��킸�Y����
			m_Position.x += (m_Position.x - characterObject->GetPosition().x) * 0.04f;
			m_Position.z += (m_Position.z - characterObject->GetPosition().z) * 0.04f;

			//�C���X�^���X��Player�ł��邩����
			if (playerObject)
			{
				if (playerObject->GetAttackStop() <= 0 && playerObject->GetInvincibleTime() <= 0)
				{
					//�v���C���[���_�b�V�����ɂԂ������ꍇ
					if (player->GetCharacterState() == CHARACTER_STATE::ALIVE &&
						player->GetPlayerState() == PLAYER_STATE::DASH)
					{
						SetDamageMove();
						playerObject->SetAttackStop(GIVE_ATTACK_STOP);
						player->AddCombo(1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g
					}
					//���ʂɂԂ������ꍇ
					else if (m_StunTime <= 0 && m_HumanState != HUMAN_STATE::MAKING)
					{
						m_HumanState = HUMAN_STATE::MAKING;
					}
				}
			}

			//�C���X�^���X��Follow�ł��邩����
			else if (followObject)
			{
				if (followObject->GetAttackStop() <= 0)
				{
					if (followObject->GetState() == FOLLOW_STATE::DASH)
					{
						SetDamageMove();
						followObject->SetAttackStop(GIVE_ATTACK_STOP);
						player->AddCombo(1);
						scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g
					}
				}
			}
		}

		//�g���Ƀv���C���[���Ė邶��Ȃ��ă��C�L���O���ĂȂ����
		if (length < TARGET_LENGTH && m_StunTime <= 0 && m_HumanState != HUMAN_STATE::MAKING && m_HumanState != HUMAN_STATE::NIGHT)
		{
			if (playerObject || (followObject && followObject->GetState() != FOLLOW_STATE::FREE)) { m_HumanState = HUMAN_STATE::MAKING; }
		}
	}


	// �j��u���b�N
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

