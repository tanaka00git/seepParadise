#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\App\input.h"
#include "..\App\audio.h"
#include "..\Scene\scene.h"
#include "..\Scene\game.h"
#include "..\GameObject\player.h"
#include "..\GameObject\box.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\score.h"
#include "..\GameObject\infoLog.h"
#include "..\GameObject\infoLogTutorial.h"
#include "..\GameObject\ConcentrationLine.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\smoke.h"
#include "..\App\model.h"

Model*Player::m_Model{};
Model*Player::m_ModelClown{};
Audio* Player::m_ShotSE{};
Audio* Player::m_WalkSE{};
Audio* Player::m_DamageSE{};
Audio* Player::m_DashSE{};
int Player::m_DebugMode{};
int Player::m_PlColor{};
int Player::m_PlClown{};

#define FULL_LIFE 9
#define FULL_SPEED 9.5
#define FULL_EYE 6
#define WALK_EFFECT_TIME 13
#define ATTACK_STOP 20

void Player::Load()
{
	m_Model = new Model();
	if (m_PlColor == 0) { m_Model->Load("asset\\model\\seep_cc7.obj"); }
	else if (m_PlColor == 1) { m_Model->Load("asset\\model\\seep_cc2.obj"); }
	else if (m_PlColor == 2) { m_Model->Load("asset\\model\\seep_cc3.obj"); }
	else if (m_PlColor == 3) { m_Model->Load("asset\\model\\seep_cc4.obj"); }
	else if (m_PlColor == 4) { m_Model->Load("asset\\model\\seep_cc5.obj"); }
	else if (m_PlColor == 5) { m_Model->Load("asset\\model\\seep_cc6.obj"); }
	else {m_Model->Load("asset\\model\\seep_c.obj");}

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
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_Shadow = AddComponent<Shadow>();
	m_HpBarS = AddComponent<HpBarS>();
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
	m_HpBarS->SetScale(m_BarScale);
}

void Player::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	GameObject::Update();
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	//����l����
	if (m_FullLife > FULL_LIFE)  { m_FullLife = FULL_LIFE; }
	if (m_Life > m_FullLife)	 { m_Life = m_FullLife; }
	if (m_Speed > FULL_SPEED)	 { m_Speed = FULL_SPEED; }
	if (m_Eye > FULL_EYE)		 { m_Eye = FULL_EYE; }
	if (m_Charge > m_FullCharge) { m_Charge = m_FullCharge; }

	//�R���{�f�[�^��n��
	m_ComboWait --;
	if (m_ComboWait <= 0) { m_ComboWait = 0; }
	if (m_PlayerState == PLAYER_STATE::DASH)
	{
		m_ComboWait = 30;
		score->SetCountCombo(m_Combo); 
	}
	else { m_Combo = 0; }

	//�̗̓f�[�^��n��
	if (m_Life <= 0) { m_Life = 0; }
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);		  //���C�t��HP�o�[�ɃZ�b�g�����
	m_HpBarS->SetCharge(m_Charge, m_FullCharge, m_Dash);  //�`���[�W��HP�o�[�ɃZ�b�g�����
	score->SetLifeF(m_Life,m_FullLife);

	//�������̏���
	if (m_Life <= 0) { m_PlayerState = PLAYER_STATE::DEATH; }

	switch (m_PlayerState)
	{
	case PLAYER_STATE::NORMAL:
		UpdateNormal();
		break;
	case PLAYER_STATE::DASH:
		UpdateDash();
		break;
	case PLAYER_STATE::DEATH:
		UpdateDeath();
		break;
	case  PLAYER_STATE::UNUSED:
		UpdateUnused();
		break;
	}

	//�d��
	m_Velocity.y -= 0.015f;

	//�����蔻��
	float groundHeight = 0.0f;
	Collision(groundHeight);

	//�ڒn
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	//�_���[�W�t���b�V��
	DamageFlash();

	//���G���Ԃ̐F�ς�
	InvincibleColor();

	//�U����~
	AttackStop();

	//�v���C���[�ʒu�̕ύX
	m_Position += m_Velocity;

	//�e�̈ړ�
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
	m_Shadow->SetScale(D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z));

	//HP�o�[�̈ړ�
	D3DXVECTOR3 HpBarPosition = m_Position;
	HpBarPosition.y += 2.0f;
	m_HpBarS->SetPosition(HpBarPosition);

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
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	// �ʏ�̕`��
	m_Model->DrawColor(m_Color, m_TextureEnable);
	m_ModelClown->DrawColor(m_Color, m_TextureEnable);

}


void Player::UpdateDeath()
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
	
		m_Scale *= 0.0f;
		m_PlayerState = PLAYER_STATE::UNUSED;
	}
}

void Player::UpdateNormal()
{
	m_Scale.y = m_OriginalScale.y;

	//�}�E�X�ړ�
	MouseTargetMove();

	m_Velocity.x = GetForward().x * (m_Speed * 0.01f);
	m_Velocity.z = GetForward().z * (m_Speed * 0.01f);

	if (Input::GetKeyPress(VK_LBUTTON) && m_Charge >= 10) { m_PlayerState = PLAYER_STATE::DASH; }
}

void Player::UpdateDash()
{
	m_Scale.y = m_OriginalScale.y;

	//�}�E�X�ړ�
	MouseTargetMove();

	if (!m_DashInit)
	{
		m_DashSE->Play(1.0f);
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<ConcentrationLine>(3);
		m_DashInit = true;
	}

	m_Velocity.x = GetForward().x * (m_Speed * 0.02f);
	m_Velocity.z = GetForward().z * (m_Speed * 0.02f);

	//�`���[�W����
	m_Charge -= 10;

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

void Player::UpdateUnused()
{
	m_Scale *= 0.0f;
}

void Player::Collision(float & groundHeight)
{
	Scene* scene = Manager::GetScene();

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
			D3DXVECTOR3 direction = m_Position - position;		//�����̂���v���C���[�܂ł̕����x�N�g��
			float abbx = D3DXVec3Dot(&direction, &right);		//X�����������v���C���[����
			float abbz = D3DXVec3Dot(&direction, &forward);		//Z�����������v���C���[����

			if (fabs(abbx) < scale.x && fabs(abbz) < scale.z)
			{
				//�_�b�V���U��
				if (m_PlayerState == PLAYER_STATE::DASH && GetAttackStop() <= 0)
				{
					breakObject->SetDamageMove();
					SetAttackStop(ATTACK_STOP);
					AddCombo(1);
				}
				if (m_Position.y < position.y + scale.y)
				{
					float penetrationX = scale.x - abs(abbx);
					float penetrationZ = scale.z - abs(abbz);

					if (penetrationX < penetrationZ)
					{
						if (abbx > 0) { m_Position += penetrationX * right; }
						else { m_Position -= penetrationX * right; }
					}
					else
					{
						if (abbz > 0) { m_Position += penetrationZ * forward; }
						else { m_Position -= penetrationZ * forward; }
					}
				}
				else { groundHeight = position.y + scale.y; }
			}
		}
	}

	//�~�n
	auto cylinders = scene->GetGameObjects<Cylinder>();
	for (Cylinder* cylinder : cylinders)
	{
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		float length = D3DXVec3Length(&direction);
		float radius = scale.x * 1.2f;  // 1.2�͒���

		if (length < radius)
		{
			if (m_Position.y < position.y + scale.y)
			{
				// �Փ˂��Ă�������ɉ����߂�
				D3DXVECTOR3 normalizedDirection = direction / length;

				float penetration = radius - length;
				m_Position += penetration * normalizedDirection;
			}
			else
			{
				groundHeight = position.y + scale.y;
			}
		}
	}


	//������
	auto boxs = scene->GetGameObjects<Box>();//���X�g���擾
	for (Box* box : boxs) //�͈�for���[�v
	{
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();
		D3DXVECTOR3 right = box->GetRight();				//x������
		D3DXVECTOR3 forward = box->GetForward();			//z������
		D3DXVECTOR3 direction = m_Position - position;		//�����̂���v���C���[�܂ł̕����x�N�g��
		float abbx = D3DXVec3Dot(&direction, &right);		//X�����������v���C���[����
		float abbz = D3DXVec3Dot(&direction, &forward);		//Z�����������v���C���[����

		//OBB
		if (fabs(abbx) < scale.x && fabs(abbz) < scale.z)
		{
			if (m_Position.y < position.y + scale.y * 1.8f - 0.5f)	//2.0f�̓��f���Œ���
			{
				float penetrationX = scale.x - abs(abbx);
				float penetrationZ = scale.z - abs(abbz);
				
				if (penetrationX < penetrationZ)
				{
					if (abbx > 0) { m_Position += penetrationX * right; }
					else { m_Position -= penetrationX * right; }
				}
				else
				{
					if (abbz > 0) { m_Position += penetrationZ * forward; }
					else { m_Position -= penetrationZ * forward; }
				}
			}
			else { groundHeight = position.y + scale.y * 1.8f; }	//�������2.0
		}
	}
}

void Player::SetDamageMove()
{
	Scene* scene = Manager::GetScene();
	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(4, 2, D3DXVECTOR3(100, -10, 0));

	//�J�����h�炷
	Camera* camera = scene->GetGameObject<Camera>();
	camera->SetShake(0.5f,0.0f);

	//SE
	m_DamageSE->Play(1.0f);
	
	m_InvincibleTime = 180;
	m_Velocity.y = 0.1f;
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

void Player::DamageFlash()
{
	if (m_DamageFlashTime > 0)
	{
		m_DamageFlashTime--;
		m_TextureEnable = false;
	}
	else { m_TextureEnable = true; }
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
		m_Velocity.x = (GetForward().x * 0.03f) * -1;
		m_Velocity.z = (GetForward().z * 0.03f) * -1;
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
		if (m_PlayerState == PLAYER_STATE::DASH) { m_Rotation.x = (0.08f * m_AnimePause); }
		else { m_Rotation.x = (0.03f * m_AnimePause); }

		//���Z�b�g
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
	}

}


