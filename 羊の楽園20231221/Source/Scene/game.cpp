#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\game.h"
#include "..\Scene\result.h"
#include "..\App\input.h"
#include "..\App\audio.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\field.h"
#include "..\GameObject\player.h"
#include "..\GameObject\human.h"
#include "..\GameObject\goal.h"
#include "..\GameObject\goalScope.h"
#include "..\GameObject\tree.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\coin.h"
#include "..\GameObject\explosion.h"
#include "..\GameObject\smoke.h"
#include "..\GameObject\shine.h"
#include "..\GameObject\shine2.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\box.h"
#include "..\GameObject\wolf.h"
#include "..\GameObject\apple.h"
#include "..\GameObject\seed.h"
#include "..\GameObject\score.h"
#include "..\GameObject\fade.h"
#include "..\GameObject\timeFade.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\fence.h"
#include "..\GameObject\mouseIcon.h"
#include "..\GameObject\itemSpeed.h"
#include "..\GameObject\itemLife.h"
#include "..\GameObject\itemEye.h"
#include "..\GameObject\hpBar.h"
#include "..\GameObject\hpBarS.h"
#include "..\GameObject\dayBar.h"
#include "..\GameObject\infoLog.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\timeFade.h"
#include "..\GameObject\concentrationLine.h"
#include "..\GameObject\daybar.h"
#include "..\GameObject\InfoLogTutorial.h"

#define CYLINDER_START_NUM 30
#define BOX_START_NUM 30
#define TREE_START_NUM 55
#define COIN_START_NUM 10
#define SEED_START_NUM 70
#define ROCK_START_NUM 10
#define ENEMY_MAKE_DISTANCE 15.0f	//�G���o������͈�
#define STAGE_MAKE_XY 110.0f
#define STAGE_MAKE_PL_XY 35.0f

void Game::Init()
{
	Apple::Load();
	Box::Load();
	Cylinder::Load();
	Tree::Load();
	Player::Load();
	Follow::Load();
	Goal::Load();
	GoalScope::Load();
	Seed::Load();
	Wolf::Load();
	Human::Load();
	Explosion::Load();
	Smoke::Load();
	Shine::Load();
	Shine2::Load();
	HpBarS::Load();
	Coin::Load();
	ItemSpeed::Load();
	ItemEye::Load();
	ItemLife::Load();
	BreakObject::Load();
	Chest::Load();
	Rock::Load();
	Fence::Load();
	DamageFade::Load();
	Shadow::Load();
	TimeFade::Load();
	ConcentrationLine::Load();
	InfoLog::Load();
	DayBar::Load();

	//�J����
	Camera*camera = AddGameObject<Camera>(0);
	camera->SetCameraState(CAMERA_STATE::GAME);

	//�t�B�[���h
	AddGameObject<Field>(1);
	
	//�S�[��
	//AddGameObject<Goal>(1);
	
	//�v���C���[
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i <= CYLINDER_START_NUM; i++) {
		Cylinder*cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetScale(D3DXVECTOR3(3.0f, 1.0f, 3.0f));
		cylinder->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));
	}

	for (int i = 0; i <= BOX_START_NUM; i++) {
		Box*box = AddGameObject<Box>(1); 
		box->SetScale(D3DXVECTOR3(3.0f, 1.0f, 3.0f));
		box->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));
	}
	for (int i = 0; i <= 23; i++) {
		Box*box = AddGameObject<Box>(1);
		box->SetScale(D3DXVECTOR3(5.0f, 2.5f, 5.0f));
		box->SetPosition(D3DXVECTOR3(5.0f * i -60.0f, -1.0f, 60.0f));
	}
	for (int i = 0; i <= 23; i++) {
		Box*box = AddGameObject<Box>(1);
		box->SetScale(D3DXVECTOR3(5.0f, 2.5f, 5.0f));
		box->SetPosition(D3DXVECTOR3(5.0f * i - 60.0f, -1.0f, -60.0f));
	}
	for (int i = 0; i <= 23; i++) {
		Box*box = AddGameObject<Box>(1);
		box->SetScale(D3DXVECTOR3(5.0f, 2.5f, 5.0f));
		box->SetPosition(D3DXVECTOR3(60.0f, -1.0f, 5.0f * i - 60.0f));
	}
	for (int i = 0; i <= 23; i++) {
		Box*box = AddGameObject<Box>(1);
		box->SetScale(D3DXVECTOR3(5.0f, 2.5f, 5.0f));
		box->SetPosition(D3DXVECTOR3(-60.0f, -1.0f, 5.0f * i - 60.0f));
	}

	//��
	for (int i = 0; i <= TREE_START_NUM; i++) { AddGameObject<Tree>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2)); }
	//�R�C��
	for (int i = 0; i <= COIN_START_NUM; i++) { AddGameObject<Coin>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));
	}
	//��
	for (int i = 0; i < SEED_START_NUM; i++) {AddGameObject<Seed>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));}
	//��
	for (int i = 0; i < ROCK_START_NUM; i++) { AddGameObject<Rock>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2)); }

	AddGameObject<MouseIcon>(3);
	m_TimeFade = AddGameObject<TimeFade>(2);
	AddGameObject<InfoLogTutorial>(2);
	AddGameObject<HpBar>(2);
	AddGameObject<Score>(2);
	AddGameObject<DayBar>(2);
	m_Fade = AddGameObject<Fade>(3);


	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\chiisanaashiato.wav");
	m_BGM->Play(1.0f,true);
	m_BGM_Night = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM_Night->Load("asset\\audio\\latenightsnow.wav");
	m_BGM_Night->Play(0.0f, true);
	m_SE_SeepCry = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_SeepCry->Load("asset\\audio\\�q�c�W�̖���.wav");
	m_SE_Bell = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_Bell->Load("asset\\audio\\���炷.wav");
	m_SE_Gong = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_Gong->Load("asset\\audio\\���̎��v���̏�.wav");
	m_SE_WolfCry = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_WolfCry->Load("asset\\audio\\�I�I�J�~�̉��i��.wav");
	m_Day = 1;
}

void Game::Uninit()
{
	Scene::Uninit();
	Apple::Unload();
	Box::Unload();
	Cylinder::Unload();
	Tree::Unload();
	Follow::Unload();
	Human::Unload();
	Goal::Unload();
	GoalScope::Unload();
	Player::Unload();
	Seed::Unload();
	Wolf::Unload();
	Explosion::Unload();
	Smoke::Unload();
	Shine::Unload();
	Shine2::Unload();
	HpBarS::Unload();
	Coin::Unload();
	BreakObject::Unload();
	Chest::Unload();
	Rock::Unload();
	Fence::Unload();
	ItemSpeed::Unload();
	ItemEye::Unload();
	ItemLife::Unload();
	DamageFade::Unload();
	Shadow::Unload();
	TimeFade::Unload();
	ConcentrationLine::Unload();
	InfoLog::Unload();
	DayBar::Unload();
}

void Game::Update()
{
	Scene::Update();

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 PLPos = player->GetPosition();
	Score* score = scene->GetGameObject<Score>();

	//BGM�̃t�F�[�h�X�V
	m_BGM->FadeUpdate();
	m_BGM_Night->FadeUpdate();

	//�b���X�V
	m_GameTime ++;
	if (m_GameTime > 60) 
	{
		TimeSecondsUpdate();
		m_GameTime = 0;
	}

	//�b���̃C�x���g
	if (m_GameTime == 0) 
	{
		//20�b��1��
		if (m_GameTimeSeconds == 20) {TimeEvent_Time20();}
		//40�b��1��
		if (m_GameTimeSeconds == 40) {TimeEvent_Time40();}
		//3�b����
		if (m_GameTimeSeconds % 2 == 0){TimeEvent_Time2Loop();}
		//4������
		if (m_Day % 3 == 0 && m_GameTimeSeconds == 0) {TimeEvent_Day3Loop();}
		//10��30�b��1��
		if (m_GameTimeSeconds == 10 || m_GameTimeSeconds == 30) { TimeEvent_Time10or30(); }
	}

	//�v���C���[���H�ׂ�ꂽ��Q�[���I�[�o�[
	if (player->GetCharacterState() == CHARACTER_STATE::UNUSED) {m_Fade->FadeOut();}
	if (m_Fade->GetFadeFinish()) {
		//���U���g�ɏ����Z�b�g����
		Result* result = scene->GetGameObject<Result>();
		result->SetDay(score->GetCountDay());
		result->SetCoin(score->GetCountCoin());
		result->SetCount(score->GetFullCount());

		Manager::SetScene<Result>();
	}

	//�f�o�b�O���[�h��p����
	bool debug = player->GetDebug();
	if (Input::GetKeyTrigger('I') && debug) { m_GameTimeSeconds = 38; }	//���ԃJ�b�g
	if (Input::GetKeyTrigger(VK_RBUTTON) && debug) { AddGameObject<Follow>(1)->SetPosition(D3DXVECTOR3(PLPos.x, 1.0f, PLPos.z));}	//�r����
}

void Game::TimeSecondsUpdate()
{
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	m_GameTimeSeconds ++;

	//���ԃf�[�^��n��
	DayBar* daybar = scene->GetGameObject<DayBar>();
	daybar->SetTimeFC(40, m_GameTimeSeconds);

	//�����C�x���g�I��
	m_NawEvent --;
	if (m_NawEvent <= 0) 
	{
		m_NawEvent = 0;
		if (m_Event1 || m_Event2) {
			m_Event1 = false;
			m_Event2 = false;
			m_SE_SeepCry->Play(1.0f, false);
			AddGameObject<InfoLog>(2)->SetNum(9, 3, D3DXVECTOR3(900, 480, 0));
		}
	}
}

void Game::TimeEvent_Time2Loop()
{
	//2�b���̃C�x���g
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 PLPos = player->GetPosition();

	for (int i = 0; i <= m_Day; i++)
	{
		int a = irand(0,5);
		if (1 == a) 
		{ 
			float rot = frand() * 2 * D3DX_PI;
			AddGameObject<Wolf>(1)->SetPosEnemyData(D3DXVECTOR3(static_cast<float>((cos(rot))) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>((sin(rot))) * ENEMY_MAKE_DISTANCE + PLPos.z), 1); }

		a = irand(0, 3);
		if (1 == a) 
		{
			float rot = frand() * 2 * D3DX_PI;
			AddGameObject<Wolf>(1)->SetPosEnemyData(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z), 2);
		}

		//3���ȏ�
		if (m_Day >= 3) 
		{
			a = irand(0, 11);
			if (1 == a) 
			{ 
				float rot = frand() * 2 * D3DX_PI;
				AddGameObject<Wolf>(1)->SetPosEnemyData(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z), 3);
			}
		}

		//4���ȏ�
		if (m_Day >= 4) 
		{
			a = irand(0, 12);
			if (1 == a) 
			{ 
				float rot = frand() * 2 * D3DX_PI;
				AddGameObject<Wolf>(1)->SetPosEnemyData(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z), 4);
			}
		}
	}
	if (m_Event1)
	{
		AddGameObject<Follow>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));
	}

	//�R�C��
	for (int i = 0; i <= 4; i++) {AddGameObject<Coin>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));}
	
	//����
	int a = irand(0, 2);
	if (1 == a) { AddGameObject<Follow>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2)); }
	
	//�l�Ԑ���
	AddGameObject<Human>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));
	//�����S
	a = irand(0, 5);
	if (1 == a) { AddGameObject<Apple>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2)); }
	
	//�`�F�X�g
	a = irand(0, 4);
	if (1 == a) { AddGameObject<Chest>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));}
	
	//��
	a = irand(0, 2);
	if (1 == a) { AddGameObject<Rock>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));}
}

void Game::TimeEvent_Time20()
{
	//��ɂ���
	m_TimeFade->FadeOut_true();
	m_SE_Gong->Play(1.0f, false);
	m_SE_WolfCry->Play(1.0f,false);
	AddGameObject<InfoLog>(2)->SetNum(2, 1, D3DXVECTOR3(340, 0, 0));
	
	m_BGM->FadeToVolume(0.0f, 0.002f);
	m_BGM_Night->FadeToVolume(0.7f, 0.002f);

	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();
	score->SetTimeZone(true);
}

void Game::TimeEvent_Time10or30()
{
	//�C�x���g�����p
	if (m_NawEvent == 0) 
	{
		if (m_Day >= 3) //3���ڂ���12����1�̊m���Ŕ�������
		{
			int a = irand(0, 11);
			if (0 == a)
			{
				a = irand(0, 3);
				if (a <= 1)
				{
					//�r�̌Q��
					AddGameObject<InfoLog>(2)->SetNum(7, 1, D3DXVECTOR3(340, 0, 0));
					m_SE_Bell->Play(1.0f, false);
					m_Event1 = true;
				}
				if (a == 2)
				{
					//�T������
					AddGameObject<InfoLog>(2)->SetNum(8, 1, D3DXVECTOR3(340, 0, 0));
					m_SE_Gong->Play(1.0f, false);
					m_Event2 = true;
				}
				m_NawEvent = 40;//40�b��܂ōĔ������Ȃ�

			}
		}
	}
}

void Game::TimeEvent_Time40()
{
	//���ɂ���
	m_TimeFade->FadeOut_false();
	m_GameTimeSeconds = 0;
	AddGameObject<InfoLog>(2)->SetNum(1, 1, D3DXVECTOR3(340, 0, 0));
	AddGameObject<InfoLog>(2)->SetNum(6, 2, D3DXVECTOR3(340, 640, 10));
	m_SE_SeepCry->Play(1.0f, false);
	m_SE_Bell->Play(1.0f, false);
	m_Day ++;

	m_BGM->FadeToVolume(1.0f, 0.002f);
	m_BGM_Night->FadeToVolume(0.0f, 0.004f);

	if (m_Day > 99) { m_Day = 99; }
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();
	score->SetCountDay(m_Day);
	score->SetTimeZone(false);
}

void Game::TimeEvent_Day3Loop()
{
	//4�����̃C�x���g
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 PLPos = player->GetPosition();

	AddGameObject<InfoLog>(2)->SetNum(13,3,D3DXVECTOR3(900,480,0));

	float rot = frand() * 2 * D3DX_PI;
	AddGameObject<Wolf>(1)->SetPosEnemyData(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z), 5);
}
