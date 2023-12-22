#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\GameObject\wolf.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\box.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\cylinder.h"
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

Model*Wolf::m_Model{};
Model*Wolf::m_Model2{};
Model*Wolf::m_Model3{};
Audio*Wolf::m_SE_Eat{};
Audio*Wolf::m_SE_Kick{};

#define EATING_TIME 40
#define DROP_RATE 20
#define APPLE_RATE 20

void Wolf::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\wolf_a.obj");
	m_Model2 = new Model();
	m_Model2->Load("asset\\model\\wolfClown1.obj");
	m_Model3 = new Model();
	m_Model3->Load("asset\\model\\wolfClown3.obj");
	m_SE_Eat = new Audio();
	m_SE_Eat->Load("asset\\audio\\eat3b.wav");
	m_SE_Kick = new Audio();
	m_SE_Kick->Load("asset\\audio\\���L�b�Nb.wav");
}

void Wolf::Unload()
{
	m_Model->Unload();
	m_Model2->Unload();
	m_Model3->Unload();
	delete m_Model;
	delete m_Model2;
	delete m_Model3;
}

void Wolf::Init()
{
	m_Scale.y = 0.01f;

	m_Rotation.y = frand() * 2 * D3DX_PI;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
	
	m_Shadow = AddComponent<Shadow>();

	m_HpBarS = AddComponent<HpBarS>();
	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);
}

void Wolf::Uninit()
{
	GameObject::Uninit();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Wolf::Update()
{
	GameObject::Update();
	Scene* scene = Manager::GetScene();

	m_HpBarS->SetLifeDateFC(m_FullLife, m_Life);

	//�����폜�܂�
	m_DaathTime --;
	if (m_DaathTime <= 0){UpdateDelete();}
	else 
	{
		m_Scale.x += 0.05f;
		m_Scale.y += 0.05f;
		m_Scale.z += 0.05f;
		if (m_Scale.x >= m_CharaScale.x) { m_Scale.x = m_CharaScale.x; }
		if (m_Scale.y >= m_CharaScale.y) { m_Scale.y = m_CharaScale.y; }
		if (m_Scale.z >= m_CharaScale.z) { m_Scale.z = m_CharaScale.z; }
	}

	//�������̏���
	if (m_Life <= 0) { m_WolfState = WOLF_STATE::DEATH; }

	switch (m_WolfState)
	{
	case WOLF_STATE::FREE:
		UpdateFree();
		break;
	case WOLF_STATE::EATING:
		UpdateEating();
		break;
	case WOLF_STATE::DAMAGE:
		UpdateDamage();
		break;
	case WOLF_STATE::TARGETING:
		UpdateTargeting();
		break;
	case WOLF_STATE::DEATH:
		UpdateDeath();
		break;
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

void Wolf::Draw()
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

	m_Model->DrawColor(m_Color, m_TextureEnable);

	if (m_Data == 5) { m_Model2->DrawColor(m_Color, m_TextureEnable); }
	if (m_Item) { m_Model3->DrawColor(m_Color, m_TextureEnable); }
}

void Wolf::UpdateFree()
{
	//�����ύX
	m_time2 += 0.1f;
	if (m_time2 > m_NextRotTime){m_Rotation.y += (0.02f * m_NextRot);}
	if (m_time2 > m_NextRotTime + 5.0f) 
	{
		int a = irand(0,2);
		if (a == 1) {
			m_NextRot *= -1;
		}
		m_NextRotTime = frand() * 30.0f + 10.0f;
		m_time2 = 0.0f;
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
	if (plength < m_Tracking || timeFade->GetTimeZone()) {m_WolfState = WOLF_STATE::TARGETING;}
}

void Wolf::UpdateTargeting()
{
	Scene* scene = Manager::GetScene();
	TimeFade* timeFade = scene->GetGameObject<TimeFade>();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	float plength = D3DXVec3Length(&direction);

	//�v���C���[�̕�������
	

	//���ԗr������ΗD��I��
	auto follows = scene->GetGameObjects<Follow>();//���X�g���擾
	Follow* nearestFollow = nullptr;
	float nearestDistance = FLT_MAX;
	for (Follow* follow : follows)
	{
		D3DXVECTOR3 position = follow->GetPosition();
		D3DXVECTOR3 scale = follow->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < m_Tracking && follow->GetState() != FOLLOW_STATE::FREE && m_WolfState != WOLF_STATE::DAMAGE)
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
	m_Velocity.x = GetForward().x * (m_Speed + (timeFede * m_Speed));
	m_Velocity.z = GetForward().z * (m_Speed + (timeFede * m_Speed));

	//�ЊQ�T��p�C�x���g
	if (m_Disaster) { DisasterMove(); }

	//�����͈͂𒴂�����
	if (plength > m_Tracking) { m_WolfState = WOLF_STATE::FREE; }//�ǔ����Ȃ�
}

void Wolf::UpdateDelete()
{
	//�ʂ�ʂ����
	m_Shadow->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Scale.x -= 0.05f; m_Scale.y -= 0.05f; m_Scale.z -= 0.05f;
	if (m_Scale.y <= 0.0f) { SetDestroy(); }
}

void Wolf::UpdateEating()
{
	m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_EatStop ++;
	if (m_EatStop % EATING_TIME == 0)
	{ 
		if(m_EatStop >= EATING_TIME * 3)
		{
			m_EatStop = 0;
			m_WolfState = WOLF_STATE::FREE;
		}
		else { m_SE_Eat->Play(1.0f); }
	}
}

void Wolf::UpdateDeath()
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
		int a = irand(0,DROP_RATE-1);
		if (a == 0) {
			a = irand(0,2);
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

void Wolf::UpdateDamage()
{
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
	m_Velocity *= 0;
	m_Rotation.y += 0.2f;

	//�U���󂯂��Ƃ��̏���
	m_DamageStop --;
	if (m_DamageStop <= 0) 
	{
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_WolfState = WOLF_STATE::FREE;
	}
}

void Wolf::Collision(float & groundHeight)
{
	if (m_WolfState == WOLF_STATE::DEATH) { return; };

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	if(player->GetInvincibleTime() <= 0)
	{
		//�v���C���[�̋������擾	
		D3DXVECTOR3 direction = m_Position - player->GetPosition();
		D3DXVECTOR3 pscale = player->GetScale();
		float plength = D3DXVec3Length(&direction);

		if (player->GetAttackStop() <= 0)
		{
			D3DXVECTOR3 scale = player->GetScale();
			if (plength < scale.x)
			{
				//�v���C���[���_�b�V�����ɂԂ������ꍇ
				if (player->GetState() == PLAYER_STATE::DASH)
				{
					SetDamageMove();
					player->SetAttackStop(20);
					player->AddCombo(1);
					scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g
				}
				//���ʂɂԂ������ꍇ
				else if (m_DamageStop <= 0 && m_WolfState != WOLF_STATE::EATING)
				{
					m_WolfState = WOLF_STATE::EATING;
					player->SetAttackStop(20);
					player->AddLife(-1);
					player->SetDamageMove();
					m_SE_Eat->Play(1.0f);
					m_Life --;
					scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g
				}
			}
		}
		
	}

	auto follows = scene->GetGameObjects<Follow>();//���X�g���擾
	for (Follow* follow : follows) {
		D3DXVECTOR3 position = follow->GetPosition();
		D3DXVECTOR3 scale = follow->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);

		if (length < scale.x) 
		{
			if (follow->GetAttackStop() <= 0) 
			{
				if (follow->GetState() == FOLLOW_STATE::DASH)
				{
					SetDamageMove();
					follow->SetAttackStop(25);
					player->AddCombo(1);
				}
				else if (m_DamageStop <= 0 && m_WolfState != WOLF_STATE::EATING) 
				{
					m_WolfState = WOLF_STATE::EATING;
					m_SE_Eat->Play(1.0f);
					m_Life --;
					follow->AddLife(-1);
					scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g
				}
			}
		}
	}

	//���̒��Ԃɏd�Ȃ�Ȃ��悤�ɂ��鏈��
	auto wolfs = scene->GetGameObjects<Wolf>();//���X�g���擾
	for (Wolf* wolf : wolfs)
	{
		D3DXVECTOR3 position = wolf->GetPosition();
		D3DXVECTOR3 scale = wolf->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float flength = D3DXVec3Length(&direction);
		if (flength < scale.x) {
			m_Position.x += (m_Position.x - wolf->GetPosition().x) * 0.02f;
			m_Position.z += (m_Position.z - wolf->GetPosition().z) * 0.02f;
		}
	}

	//���b�N
	auto rocks = scene->GetGameObjects<Rock>();//���X�g���擾
	for (Rock* rock : rocks) {
		if (rock->GetState() == BREAKOBJECT_STATE::NORMAL) {
			D3DXVECTOR3 position = rock->GetPosition();
			D3DXVECTOR3 scale = rock->GetScale();
			if (position.x - scale.x < m_Position.x && m_Position.x < position.x + scale.x &&
				position.z - scale.z < m_Position.z && m_Position.z < position.z + scale.z) {
				if (m_Position.y < position.y + scale.y) {//2.0f�̓��f���Œ���
					m_Velocity.x = (m_Position.x - position.x) * 0.02f;
					m_Velocity.z = (m_Position.z - position.z) * 0.02f;
				}
				else { groundHeight = position.y + scale.y; }	//�������2.0
			}
		}
	}
	//�`�F�X�g
	auto chests = scene->GetGameObjects<Chest>();//���X�g���擾
	for (Chest* chest : chests) {
		if (chest->GetState() == BREAKOBJECT_STATE::NORMAL) {
			D3DXVECTOR3 position = chest->GetPosition();
			D3DXVECTOR3 scale = chest->GetScale();
			if (position.x - scale.x < m_Position.x && m_Position.x < position.x + scale.x &&
				position.z - scale.z < m_Position.z && m_Position.z < position.z + scale.z) {
				if (m_Position.y < position.y + scale.y) {//2.0f�̓��f���Œ���
					m_Velocity.x = (m_Position.x - position.x) * 0.02f;
					m_Velocity.z = (m_Position.z - position.z) * 0.02f;
				}
				else { groundHeight = position.y + scale.y; }	//�������2.0
			}
		}
	}
	//�~�n
	auto cylinders = scene->GetGameObjects<Cylinder>();//���X�g���擾
	for (Cylinder* cylinder : cylinders) {
		D3DXVECTOR3 position = cylinder->GetPosition();
		D3DXVECTOR3 scale = cylinder->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		direction.y = 0.0f;
		float length = D3DXVec3Length(&direction);
		if (length < scale.x * 1.2f) {//*1.2�͒���
			if (m_Position.y < position.y + scale.y - 0.5f) {
				m_Velocity.x = (m_Position.x - position.x) * 0.02f;
				m_Velocity.z = (m_Position.z - position.z) * 0.02f;
			}
			else { groundHeight = position.y + scale.y; }
		}
	}
	//������
	auto boxs = scene->GetGameObjects<Box>();//���X�g���擾
	for (Box* box : boxs) {
		D3DXVECTOR3 position = box->GetPosition();
		D3DXVECTOR3 scale = box->GetScale();

		if (position.x - scale.x < m_Position.x && m_Position.x < position.x + scale.x &&
			position.z - scale.z < m_Position.z && m_Position.z < position.z + scale.z) {
			if (m_Position.y < position.y + scale.y * 1.8f - 0.5f) {//2.0f�̓��f���Œ���
				m_Velocity.x = (m_Position.x - position.x) * 0.01f;
				m_Velocity.z = (m_Position.z - position.z) * 0.01f;
			}
			else { groundHeight = position.y + scale.y * 1.8f; }
		}
	}
}

void Wolf::KnockBack()
{
	//���
	if (m_Back >= 1) {
		m_Back --;
		m_Velocity.x += (m_PLForward.x * 0.05f);
		m_Velocity.z += (m_PLForward.z * 0.05f);
		if (m_Back >= 14) {
			m_Velocity.x += (m_PLForward.x * 0.3f);
			m_Velocity.z += (m_PLForward.z * 0.3f);
			if (m_Back <= 0) { m_Back = 0; }
		}
	}
}

void Wolf::DisasterMove()
{
	//�������ł��Ȃ��悤�ɓK���Ȓl�ŏ㏑��
	m_DaathTime = 1000;

	//��莞�Ԍo�ƃI�[�g�œG�𐶂ݏo��
	Scene* scene = Manager::GetScene();
	m_DisasterCount ++;
	if (m_DisasterCount == 150) 
	{
		Wolf* wolf = scene->AddGameObject<Wolf>(1);
		wolf->SetEnemyData(1);
		wolf->SetPosition(m_Position);
		m_DisasterCount = 0;
	}
}

void Wolf::DamageFlash()
{
	m_ColorChange--;
	if (m_ColorChange > 0)
	{
		m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_TextureEnable = false;
	}
	else
	{
		m_TextureEnable = true;
		m_ColorChange = 0;
	}
}

void Wolf::Anime()
{
	m_AnimeTime++;
	if (m_AnimeTime > 7)
	{
		//�X����
		if (m_WolfState == WOLF_STATE::TARGETING) { m_Rotation.x = (0.16f * m_AnimePause); }
		else { m_Rotation.x = (0.04f * m_AnimePause); }

		//���Z�b�g
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0;
	}
}

void Wolf::SetDamageMove()
{
	m_DamageStop = 100;
	m_WolfState = WOLF_STATE::DAMAGE;

	m_Velocity.y = 0.1f;
	m_Back = 10;
	m_Life --;
	m_SE_Kick->Play(1.0f);
	m_ColorChange = 5;

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	m_PLForward = player->GetForward();
	scene->AddGameObject<Explosion>(1)->SetPosition(m_Position);//�����G�t�F�N�g

	Camera*camera = scene->GetGameObject<Camera>();
	camera->SetShake(0.05f);
}

void Wolf::SetPosEnemyData(D3DXVECTOR3 Position, int Num)
{
	m_Position = Position;
	SetEnemyData(Num);
}

void Wolf::SetEnemyData(int data)
{
	m_Data = data;

	//��m���Ń����S�𓪂ɏ悹��
	int dropPercent = irand(0,19);
	if (dropPercent == 0) { m_Item = true; }

	if (m_Data == 1)
	{
		m_FullLife = 5;
		m_Speed = 0.025f;
		m_CoinDrop = 1;
		m_CharaScale = D3DXVECTOR3(0.6f, 0.6f, 0.6f);	//�L�����̃T�C�Y
		m_BarScale = D3DXVECTOR3(0.5f, 0.7f, 0.7f);		//HP�o�[�̃T�C�Y
		m_HpBarPosY = 1.8f;
	}
	else if (m_Data == 2)
	{
		m_FullLife = 7;
		m_Speed = 0.025f;
		m_CoinDrop = 2;
		m_CharaScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_BarScale = D3DXVECTOR3(0.7f, 0.7f, 0.7f);	
		m_HpBarPosY = 1.8f;

	}
	else if (m_Data == 3)
	{
		m_FullLife = 13;
		m_Speed = 0.03f;
		m_CoinDrop = 3;
		m_CharaScale = D3DXVECTOR3(1.3f, 1.3f, 1.3f);
		m_BarScale = D3DXVECTOR3(1.0f, 0.7f, 0.7f);
		m_HpBarPosY = 2.2f;
	}
	else if (m_Data == 4)
	{
		m_FullLife = 15;
		m_Speed = 0.03f;
		m_CoinDrop = 4;
		m_CharaScale = D3DXVECTOR3(1.7f, 1.7f, 1.7f);
		m_BarScale = D3DXVECTOR3(1.4f, 0.7f, 0.7f);
		m_HpBarPosY = 2.8f;
	}
	else if (m_Data == 5) 
	{
		m_Item = false;
		m_FullLife = 80;
		m_Speed = 0.04f;
		m_CoinDrop = 30;
		m_CharaScale = D3DXVECTOR3(2.9f, 2.9f, 2.9f);	//�L�����̃T�C�Y
		m_BarScale = D3DXVECTOR3(5.0f, 1.0f, 1.0f);		//HP�o�[�̃T�C�Y
		m_Tracking = 200.0f;							//�ǔ��͈�
		m_Disaster = true;
	}
	m_Life = m_FullLife;
	
}

