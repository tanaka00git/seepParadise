#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\App\input.h"
#include "..\App\audio.h"
#include "..\Scene\scene.h"
#include "..\Scene\game.h"
#include "..\GameObject\characterObject.h"
#include "..\GameObject\player.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\goal.h"
#include "..\GameObject\goalNavigation.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\score.h"
#include "..\GameObject\infoLog.h"
#include "..\GameObject\infoLogTutorial.h"
#include "..\GameObject\ConcentrationLine.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\smoke.h"
#include "..\GameObject\angelRing.h"
#include "..\App\model.h"

Model*Player::m_Model{};
Model*Player::m_ModelClown{};
Audio* Player::m_ShotSE{};
Audio* Player::m_WalkSE{};
Audio* Player::m_DamageSE{};
Audio* Player::m_DashSE{};
GoalNavigation* Player::m_GoalNavigation{};
int Player::m_DebugMode{};
int Player::m_PlColor{};
int Player::m_PlClown{};

#define INITIAL_LIFE 3
#define INITIAL_INVINCIBLE_TIME 0
#define INITIAL_DASH_INIT false
#define INITIAL_DASH false
#define INITIAL_COMBO 0
#define INITIAL_DEATH_STAGING 0.14f
#define INITIAL_ATTACK_STOP_TIME 0
#define INITIAL_CHARGE 0
#define INITIAL_OLD_CHARGE 0
#define INITIAL_FULL_CHARGE 2000
#define INITIAL_TUTORIAL_END false
#define INITIAL_DELETE_INIT false
#define INITIAL_ANIME_PAUSE true
#define INITIAL_ANIME_TIME 0
#define INITIAL_ANIME_ROTATION_X 0.0f
#define INITIAL_WALK_EFFECT_TIME 0
#define INITIAL_EYE 0

#define FULL_LIFE 9
#define FULL_SPEED 9.5
#define FULL_EYE 6
#define WALK_EFFECT_TIME 13
#define ATTACK_STOP 22
#define GRAVITY 0.015f
#define CHARGE_DOWN 7

void Player::Load()
{
	m_Model = new Model();
	if (m_PlColor == 0) { m_Model->Load("asset\\model\\sheep7.obj"); }
	else if (m_PlColor == 1) { m_Model->Load("asset\\model\\sheep2.obj"); }
	else if (m_PlColor == 2) { m_Model->Load("asset\\model\\sheep3.obj"); }
	else if (m_PlColor == 3) { m_Model->Load("asset\\model\\sheep4.obj"); }
	else if (m_PlColor == 4) { m_Model->Load("asset\\model\\sheep5.obj"); }
	else if (m_PlColor == 5) { m_Model->Load("asset\\model\\sheep6.obj"); }
	else {m_Model->Load("asset\\model\\sheep1.obj");}

	m_ModelClown = new Model();
	if (m_PlClown == 0) { m_ModelClown->Load("asset\\model\\clown.obj"); }
	else if (m_PlClown == 1) { m_ModelClown->Load("asset\\model\\clown2.obj"); }
	else if (m_PlClown == 2) { m_ModelClown->Load("asset\\model\\clown3.obj"); }
	else if (m_PlClown == 3) { m_ModelClown->Load("asset\\model\\clown7.obj"); }
	else if (m_PlClown == 4) { m_ModelClown->Load("asset\\model\\clown5.obj"); }
	else if (m_PlClown == 5) { m_ModelClown->Load("asset\\model\\clown6.obj"); }
	else { m_ModelClown->Load("asset\\model\\clown4.obj"); }

	m_ShotSE = new Audio();
	m_ShotSE->Load("asset\\audio\\�L�����Z��3.wav");
	m_WalkSE = new Audio();
	m_WalkSE->Load("asset\\audio\\�X���b�p�ŕ���.wav");
	m_DamageSE = new Audio();
	m_DamageSE->Load("asset\\audio\\�Ō�8.wav");
	m_DashSE = new Audio();
	m_DashSE->Load("asset\\audio\\�����ړ�.wav");
}

void Player::Unload()
{
	m_Model->Unload();
	m_ModelClown->Unload();
	delete m_Model;
	delete m_ModelClown;
	m_Model = nullptr;
	m_ModelClown = nullptr;
}

void Player::Init()
{
	CharacterObject::Init();

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	//�萔�ŏ�����
	m_Life = INITIAL_LIFE;
	m_FullLife = INITIAL_LIFE;
	m_InvincibleTime = INITIAL_INVINCIBLE_TIME;
	m_DashInit = INITIAL_DASH_INIT;
	m_Dash = INITIAL_DASH;
	m_Combo = INITIAL_COMBO;
	m_DeathStaging = INITIAL_DEATH_STAGING;
	m_AttackStopTime = INITIAL_ATTACK_STOP_TIME;
	m_Charge = INITIAL_CHARGE;
	m_OldCharge = INITIAL_OLD_CHARGE;
	m_FullCharge = INITIAL_FULL_CHARGE;
	m_TutorialEnd = INITIAL_TUTORIAL_END;
	m_DeleteInit = INITIAL_DELETE_INIT;
	m_AnimePause = INITIAL_ANIME_PAUSE;
	m_AnimeTime = INITIAL_ANIME_TIME;
	m_AnimeRotationX = INITIAL_ANIME_ROTATION_X;
	m_WalkEffectTime = INITIAL_WALK_EFFECT_TIME;
	m_Eye = INITIAL_EYE;

	//�A�^�b�`
	m_Shadow = AddComponent<Shadow>();

	Scene* scene = Manager::GetScene();
	m_GoalNavigation = scene->AddGameObject<GoalNavigation>(1);
	m_HpBarS->SetDraw(true);
}

void Player::Uninit()
{
	CharacterObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	CharacterObject::Update();

	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	//����l����
	if (m_FullLife > FULL_LIFE) { m_FullLife = FULL_LIFE; }
	if (m_Life > m_FullLife) { m_Life = m_FullLife; }
	if (m_Speed > FULL_SPEED) { m_Speed = FULL_SPEED; }
	if (m_Eye > FULL_EYE) { m_Eye = FULL_EYE; }
	if (m_Charge > m_FullCharge) { m_Charge = m_FullCharge; }
	
	//�Œ�l
	if (m_Life <= 0) { m_Life = 0; }

	//�R���{�f�[�^��n��
	if (m_PlayerState == PLAYER_STATE::DASH)
	{
		score->SetCountCombo(m_Combo);
	}
	else { m_Combo = 0; }

	//�`���[�W�ƍ��ド�C�t��HP�o�[�ɃZ�b�g����
	m_HpBarS->SetCharge(m_Charge, m_FullCharge, m_Dash); 
	score->SetLifeF(m_Life, m_FullLife);

	//HP�o�[�̈ړ�
	D3DXVECTOR3 HpBarPosition = m_Position;
	HpBarPosition.y += 2.0f;
	m_HpBarS->SetPosition(HpBarPosition);


	//�U����~
	AttackStop();

	//�d��
	m_Velocity.y -= GRAVITY;
	
	//�ړ�
	m_Position += m_Velocity;

	//�����蔻��
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//���G���Ԃ̐F�ς�
	InvincibleColor();

	//�e�̈ړ�
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
	m_Shadow->SetScale(D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z));


	//�S�[���i�r�̈ړ�
	Goal* goal = scene->GetGameObject<Goal>();
	D3DXVECTOR3 goalNavigationRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 goalNavigationPosition = m_Position;
	goalNavigationRotation.y = atan2f((goal->GetPosition().x) - m_Position.x, (goal->GetPosition().z) - m_Position.z);
	goalNavigationPosition.x += m_GoalNavigation->GetForward().x * 5;
	goalNavigationPosition.z += m_GoalNavigation->GetForward().z * 5;
	m_GoalNavigation->SetPosition(goalNavigationPosition);
	m_GoalNavigation->SetRotation(goalNavigationRotation);

	//�^���A�j��
	Anime();

	//�`���[�g���A���e�L�X�g�\��
	if (!m_TutorialEnd) { TutorialText(); }
}

void Player::Draw()
{
	GameObject::Draw();
	// �ʏ�̕`��
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

	// �ʏ�̕`��
	m_Model->DrawColor(m_Color, m_TextureEnable);
	m_ModelClown->DrawColor(m_Color, m_TextureEnable);
}

void Player::UpdateAlive()
{
	CharacterObject::UpdateAlive();

	//�}�E�X�ړ�
	MouseTargetMove();

	switch (m_PlayerState)
	{
	case PLAYER_STATE::NORMAL:
		UpdateNormal();
		break;
	case PLAYER_STATE::DASH:
		UpdateDash();
		break;
	}
}

void Player::UpdateDead()
{
	m_Velocity *= 0;
	m_Rotation.z += m_DeathStaging / 2.0f;
	m_Rotation.y -= m_DeathStaging;
	m_Scale.x -= 0.005f; m_Scale.y -= 0.005f; m_Scale.z -= 0.005f;
	m_DeathStaging -= 0.001f;

	if (m_Rotation.z > D3DX_PI / 4) { m_Rotation.z = D3DX_PI / 4; }
	if (m_DeathStaging < 0.0f) { m_DeathStaging = 0.0f; }
	if (m_Scale.y < 0.0f)
	{
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<AngelRing>(1)->SetPosition(m_Position);//�V�g�G�t�F�N�g
		m_WalkEffectTime = 0;

		m_Scale *= 0.0f;
		m_CharacterState = CHARACTER_STATE::UNUSED;
	}
}

void Player::UpdateNormal()
{
	SmoothAppearance(true);	//�ʂ�ʂ�o��

	m_Velocity.x = GetForward().x * (m_Speed * 0.01f);
	m_Velocity.z = GetForward().z * (m_Speed * 0.01f);
	
	if (Input::GetKeyPress(VK_LBUTTON) && m_Charge >= 10) { m_PlayerState = PLAYER_STATE::DASH; }
}

void Player::UpdateDash()
{
	SmoothAppearance(true);	//�ʂ�ʂ�o��

	//�_�b�V������
	if (!m_DashInit)
	{
		m_DashSE->Play(1.0f);
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<ConcentrationLine>(3);
		m_DashInit = true;
	}

	//�ړ�
	m_Velocity.x = GetForward().x * (m_Speed * 0.02f);
	m_Velocity.z = GetForward().z * (m_Speed * 0.02f);

	//�`���[�W����
	m_Charge -= CHARGE_DOWN;

	//�G�t�F�N�g
	WalkEffect();

	//�_�b�V���I��
	if (m_Charge <= 0)
	{
		m_Charge = 0;
		m_DashInit = false;
		m_PlayerState = PLAYER_STATE::NORMAL;
	}

	if (!Input::GetKeyPress(VK_LBUTTON)) 
	{ 
		m_DashInit = false;
		m_PlayerState = PLAYER_STATE::NORMAL;
	}
}

void Player::SetKnockBack()
{
	m_Velocity.y += 0.1f;
	m_AttackStopTime = ATTACK_STOP;
}

void Player::SetDamageMove()
{
	CharacterObject::SetDamageMove();

	Scene* scene = Manager::GetScene();
	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(4, 2, D3DXVECTOR3(100, -10, 0));

	//�J�����h�炷
	Camera* camera = scene->GetGameObject<Camera>();
	camera->SetShake(0.5f,0.0f);

	//SE
	m_DamageSE->Play(1.0f);
	
	m_InvincibleTime = 180;
	DamageFade* damageFade = scene->AddGameObject<DamageFade>(2);
	damageFade->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.3f));
}

void Player::WalkEffect()
{
	m_WalkEffectTime++;

	//�G�t�F�N�g�����ʒu����(�v���C���[�̌��)
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
		scene->AddGameObject<Smoke>(1)->SetPosition(effectPosition);//�����G�t�F�N�g
		m_WalkEffectTime = 0;
	}
}

void Player::TutorialText()
{
	Scene* scene = Manager::GetScene();
	InfoLogTutorial* infoLogTutorial = scene->GetGameObject<InfoLogTutorial>();
	if (infoLogTutorial)
	{
		int n = infoLogTutorial->GetTutorialLog();
		if (n == 2)
		{
			if (m_Charge > m_OldCharge) { infoLogTutorial->LogChargeTrue(); }
		}
		if (n == 3)
		{
			if (m_OldCharge > m_Charge) { infoLogTutorial->LogAttackTrue(); m_TutorialEnd = true; }
		}
		m_OldCharge = m_Charge;
	}
}

void Player::MouseTargetMove()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXMATRIX viewMatrix = camera->GetViewMatrix();

	// �}�E�X�̍��W���擾
	POINT mousePos = Input::GetMousePosition();

	// �}�E�X���W�𐳋K���f�o�C�X���W�n�ɕϊ�
	float SCREEN_WIDTH_FLOAT = static_cast<float>(SCREEN_WIDTH);
	float SCREEN_HEIGHT_FLOAT = static_cast<float>(SCREEN_HEIGHT);
	float normalizedX = (2.0f * static_cast<float>(mousePos.x)) / SCREEN_WIDTH_FLOAT - 1.0f;
	float normalizedY = 1.0f - (2.0f * static_cast<float>(mousePos.y)) / SCREEN_HEIGHT_FLOAT - 1.0f;

	// �r���[�s��̋t�s����擾
	D3DXMATRIX inverseViewMatrix;
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// �r���[���W�n�ł̕����x�N�g�����v�Z
	D3DXVECTOR3 directionView;
	directionView.x = normalizedX;
	directionView.y = normalizedY;
	directionView.z = 1.0f;

	// �r���[���W�n���烏�[���h���W�n�֕ϊ�
	D3DXVECTOR3 directionWorld;
	D3DXVec3TransformNormal(&directionWorld, &directionView, &inverseViewMatrix);

	// ���[���h���W�n�ł̃}�E�X�̕����x�N�g�����擾
	D3DXVECTOR3 mouseDirection = D3DXVECTOR3(directionWorld.x, directionWorld.y, directionWorld.z);
	D3DXVec3Normalize(&mouseDirection, &mouseDirection);

	// �v���C���[�������������v�Z
	D3DXVECTOR3 toLookAt = mouseDirection;
	D3DXVec3Normalize(&toLookAt, &toLookAt);
	m_Rotation.y = atan2f(toLookAt.x, toLookAt.z);
}

void Player::InvincibleColor()
{
	if (m_InvincibleTime > 0)
	{
		m_InvincibleTime--;
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	}
	else { m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
}

void Player::AttackStop()
{
	m_AttackStopTime--;
	if (m_AttackStopTime > 0)	//�U����~(�܂��U���ł���܂�)
	{
		m_Velocity.x = (GetForward().x * 0.075f) * -1;
		m_Velocity.z = (GetForward().z * 0.075f) * -1;
	}
	if (m_AttackStopTime <= 0) { m_AttackStopTime = 0; }
}

void Player::Anime()
{
	m_AnimeTime++;
	int time = 14;
	if (m_PlayerState == PLAYER_STATE::DASH) { time = 4; }

	if (m_AnimeTime > time)
	{
		if (m_AnimePause) { m_WalkSE->Play(1.0f); }

		//�X����
		if (m_PlayerState == PLAYER_STATE::DASH) { m_AnimeRotationX = (0.08f * m_AnimePause); }
		else { m_AnimeRotationX = (0.03f * m_AnimePause); }

		//���Z�b�g
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
	}
}

void Player::Collision(float& groundHeight)
{
	CharacterObject::Collision(groundHeight);
	Scene* scene = Manager::GetScene();

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
				// �_�b�V���U��
				if (m_PlayerState == PLAYER_STATE::DASH && GetAttackStop() <= 0)
				{
					breakObject->SetDamageMove();
					AddCombo(1);
					SetKnockBack();
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