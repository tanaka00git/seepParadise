#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\title.h"
#include "..\Scene\game.h"
#include "..\Scene\result.h"
#include "..\App\input.h"
#include "..\GameObject\titleLogo.h"
#include "..\GameObject\titleScore.h"
#include "..\GameObject\fade.h"
#include "..\GameObject\timeFade.h"
#include "..\App\audio.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\mouseIcon.h"
#include "..\GameObject\titleSeep.h"
#include "..\GameObject\titleCoin.h"
#include "..\GameObject\titleBox.h"
#include "..\GameObject\titleSky.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\infoLog.h"
#include "..\GameObject\player.h"


bool Title::m_Begin{};
bool Title::m_DebugMode{};
int Title::m_Coin{};
int Title::m_PlColor{};
int Title::m_PlClown{};

void Title::Init()
{
	//�N�������菈��
	if (!m_Begin) 
	{
		m_DebugMode = false;
		m_PlColor = 6;
		m_PlClown = 6;
		m_Coin = 200;
		m_Begin = true;
	}

	//�R�C��������𒴂��Ă�����
	if (m_Coin > 99999) { m_Coin = 99999; }
	
	TitleSeep::Load();
	TitleBox::Load();
	TitleSky::Load();
	TimeFade::Load();
	DamageFade::Load();

	Camera*camera = AddGameObject<Camera>(0);
	camera->SetCameraState(CAMERA_STATE::TITLE);

	AddGameObject<TitleSky>(0);
	AddGameObject<TitleBox>(1);
	AddGameObject<MouseIcon>(3);
	
	m_TimeFade = AddGameObject<TimeFade>(2);
	m_TitleLogo = AddGameObject<TitleLogo>(2);
	m_TitleScore = AddGameObject<TitleScore>(2);
	m_TitleScore->SetCountCoin(m_Coin);
	m_TitleScore->SetColorClown(m_PlColor, m_PlClown);//�r�̗e�p�̃f�[�^�𑗂�

	m_Fade = AddGameObject<Fade>(3);

	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\moon.wav");
	m_BGM->Play(1.0f,true);
	
	m_PointSE = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_PointSE->Load("asset\\audio\\����{�^��������28.wav");
	m_PointMoveSE = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_PointMoveSE->Load("asset\\audio\\�J�[�\���ړ�9.wav");
	m_LegiSE = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_LegiSE->Load("asset\\audio\\���W�X�^�[�Ő��Z.wav");
	m_MissSE = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_MissSE->Load("asset\\audio\\�r�[�v��2.wav");
	m_SE = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE->Load("asset\\audio\\�q�c�W�̖���.wav");
}

void Title::Uninit()
{
	Scene::Uninit();

	TitleBox::Unload();
	TitleSky::Unload();
	TimeFade::Unload();
	DamageFade::Unload();

}

void Title::Update()
{
	Scene::Update();

	m_SeepDropTime++;
	if (m_SeepDropTime > 40)
	{
		AddGameObject<TitleSeep>(1);
		m_SeepDropTime = 0;
	}

	switch (m_TitleState)
	{
	case TITLE_STATE::PRESS_START:
		UpdatePressStart();
		break;
	case TITLE_STATE::TARGET_GAME:
		UpdateTargetGame();
		break;
	case TITLE_STATE::TARGET_SHOP:
		UpdateTargetShop();
		break;
	case TITLE_STATE::TARGET_RULE:
		UpdateTargetRule();
		break;
	case TITLE_STATE::FADE:
		UpdateFade();
		break;
	}

	//�L�[���͌n
	int TitleScene = m_TitleLogo->GetScene();

	//�f�o�b�O���[�h��p����
	if (Input::GetKeyTrigger(VK_RBUTTON) && m_DebugMode)	//�f�o�b�O�ŃR�C���𑝂₷
	{ 
		m_LegiSE->Play(1.0f);
		m_Coin += 100;
		m_TitleScore->SetCountCoin(m_Coin);	//�R�C���`����X�V
	}
	m_TitleLogo->SetDebug(m_DebugMode);

}

void Title::UpdatePressStart()
{
	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
		m_PointSE->Play(1.0f);
		m_TitleLogo->SetScene(1);
		m_TitleState = TITLE_STATE::TARGET_GAME;
	}
}

void Title::UpdateTargetGame()
{
	//�X�^�[�g
	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
	m_PointSE->Play(1.0f);
	m_SE->Play(1.0f);
	m_TitleLogo->SetScene(3);
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();//�v���C���[���
	player->SetColorClown(m_PlColor, m_PlClown);	//�����ڕύX
	player->SetDebugMode(m_DebugMode);				//�f�o�b�O���[�h�ݒ�
	m_Fade->FadeOut();								//�t�F�[�h�J�n
	m_TitleState = TITLE_STATE::FADE;
	}

	//�^�[�Q�b�g�𓮂���
	if (m_TitleState != TITLE_STATE::FADE) { TargetMove(); }

}

void Title::UpdateTargetShop()
{

	//�V���b�v
	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
		if (m_Coin >= 50) //�R�C����50����΍w��
		{
			AddGameObject<TitleCoin>(3);
			AddGameObject<TitleCoin>(3);
			AddGameObject<TitleCoin>(3);

			m_PointSE->Play(1.0f);
			m_LegiSE->Play(1.0f);
			Scene* scene = Manager::GetScene();
			DamageFade* damageFade = scene->AddGameObject<DamageFade>(2);
			damageFade->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
			InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
			infoLog->SetNum(44, 2, D3DXVECTOR3(460, 370, 0));
			m_Coin -= 50;
			m_PlColor = irand(0,6);
			m_PlClown = irand(0,6);
			m_TitleScore->SetColorClown(m_PlColor, m_PlClown);//�r�̗e�p�̃f�[�^�𑗂�
			m_TitleScore->SetCountCoin(m_Coin);				  //�R�C���`����X�V

		}
		else { m_MissSE->Play(1.0f); }	//�w���ł��Ȃ�����
	}

	//�^�[�Q�b�g�𓮂���
	TargetMove();
}

void Title::UpdateTargetRule()
{
	//�w���v�\��(�f�o�b�O�N��)
	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
		m_PointSE->Play(1.0f);
		m_DebugMode = !m_DebugMode;
	}

	//�^�[�Q�b�g�𓮂���
	TargetMove();
}

void Title::UpdateFade()
{
	//�t�F�[�h�I�����Ɏ��̃V�[���ֈڍs
	if (m_Fade->GetFadeFinish()) { Manager::SetScene<Game>(); }
}

void Title::TargetMove()
{	
	TITLE_STATE oldTitleState = m_TitleState;
	//�E��
	if (Input::IsMouseClickInArea(230, 470, 340, 500))
	{
		m_TitleLogo->SetSelect(1);
		m_TitleState = TITLE_STATE::TARGET_GAME;
	}
	else if (Input::IsMouseClickInArea(430, 470, 540, 500))
	{
		m_TitleLogo->SetSelect(2);
		m_TitleState = TITLE_STATE::TARGET_SHOP;
	}
	else if (Input::IsMouseClickInArea(630, 470, 740, 500))
	{
		m_TitleLogo->SetSelect(3);
		m_TitleState = TITLE_STATE::TARGET_RULE;
	}

	if (m_TitleState != oldTitleState)
	{
		m_PointMoveSE->Play(1.0f);
		if (m_TitleLogo->GetSelect() == 2) { m_TitleScore->SetAlpha(true); }
		else { m_TitleScore->SetAlpha(false); }
	}
}
