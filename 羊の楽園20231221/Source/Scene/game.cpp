#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\game.h"
#include "..\Scene\result.h"
#include "..\App\input.h"
#include "..\App\audio.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\field.h"
#include "..\GameObject\goalNavigation.h"
#include "..\GameObject\player.h"
#include "..\GameObject\human.h"
#include "..\GameObject\humanSpring.h"
#include "..\GameObject\goal.h"
#include "..\GameObject\goalScope.h"
#include "..\GameObject\tree.h"
#include "..\GameObject\spring.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\feet.h"
#include "..\GameObject\explosion.h"
#include "..\GameObject\smoke.h"
#include "..\GameObject\angelRing.h"
#include "..\GameObject\shine.h"
#include "..\GameObject\shine2.h"
#include "..\GameObject\goalText.h"
#include "..\GameObject\feetEffect.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\box.h"
#include "..\GameObject\wolf.h"
#include "..\GameObject\DisasterWolf.h"
#include "..\GameObject\apple.h"
#include "..\GameObject\seed.h"
#include "..\GameObject\score.h"
#include "..\GameObject\fade.h"
#include "..\GameObject\timeFade.h"
#include "..\GameObject\goal.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\sheepCage.h"
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
#include "..\GameObject\attackMarker.h"
#include "..\GameObject\timeFade.h"
#include "..\GameObject\concentrationLine.h"
#include "..\GameObject\daybar.h"
#include "..\GameObject\InfoLogTutorial.h"

const int CYLINDER_START_NUM = 30;
const int BOX_START_NUM = 30;
const int TREE_START_NUM = 20;
const int FEET_START_NUM = 10;
const int SEED_START_NUM = 70;
const int ROCK_START_NUM = 10;
const float ENEMY_MAKE_DISTANCE = 18.0f;  // 敵が出現する範囲
const float STAGE_MAKE_XY = 100.0f;
const float STAGE_MAKE_PL_XY = 35.0f;

void Game::Init()
{
	Apple::Load();
	Box::Load();
	Cylinder::Load();
	Tree::Load();
	Spring::Load();
	GoalNavigation::Load();
	Player::Load();
	Follow::Load();
	Goal::Load();
	GoalScope::Load();
	Seed::Load();
	Wolf::Load();
	DisasterWolf::Load();
	Human::Load();
	HumanSpring::Load();
	Explosion::Load();
	Smoke::Load();
	AngelRing::Load();
	Shine::Load();
	Shine2::Load();
	GoalText::Load();
	FeetEffect::Load();
	HpBarS::Load();
	Feet::Load();
	ItemSpeed::Load();
	ItemEye::Load();
	ItemLife::Load();
	BreakObject::Load();
	Goal::Load();
	Chest::Load();
	SheepCage::Load();
	Rock::Load();
	Fence::Load();
	DamageFade::Load();
	Shadow::Load();
	AttackMarker::Load();
	TimeFade::Load();
	ConcentrationLine::Load();
	DayBar::Load();

	m_Day = 1;

	//カメラ
	Camera*camera = AddGameObject<Camera>(0);
	camera->SetCameraState(CAMERA_STATE::GAME);

	//フィールド
	AddGameObject<Field>(1);

	//プレイヤー
	Player* player = AddGameObject<Player>(1);
	player->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//ゴール
	AddGameObject<Goal>(1);

	//箱型地面生成
	for (int i = 0; i <= CYLINDER_START_NUM; i++) {
		Cylinder*cylinder = AddGameObject<Cylinder>(1);
		cylinder->SetScale(D3DXVECTOR3(3.0f, 1.0f, 3.0f));
		cylinder->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));
	}
	for (int i = 0; i <= BOX_START_NUM; i++) {
		Box* box = AddGameObject<Box>(1);
		box->SetScale(D3DXVECTOR3(3.0f, 1.0f, 3.0f));
		box->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));
	}

	for (int i = 0; i <= 12; i++) {
		Box* box1 = AddGameObject<Box>(1);
		Box* box2 = AddGameObject<Box>(1);
		Box* box3 = AddGameObject<Box>(1);
		Box* box4 = AddGameObject<Box>(1);

		box1->SetScale(D3DXVECTOR3(10.0f, 3.5f, 10.0f));
		box2->SetScale(D3DXVECTOR3(10.0f, 3.5f, 10.0f));
		box3->SetScale(D3DXVECTOR3(10.0f, 3.5f, 10.0f));
		box4->SetScale(D3DXVECTOR3(10.0f, 3.5f, 10.0f));

		box1->SetPosition(D3DXVECTOR3(10.0f * i - 70.0f, -1.0f, 60.0f));
		box2->SetPosition(D3DXVECTOR3(10.0f * i - 70.0f, -1.0f, -60.0f));
		box3->SetPosition(D3DXVECTOR3(60.0f, -1.0f, 10.0f * i - 60.0f));
		box4->SetPosition(D3DXVECTOR3(-60.0f, -1.0f, 10.0f * i - 60.0f));
	}


	//木
	for (int i = 0; i <= TREE_START_NUM; i++) { AddGameObject<Tree>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2)); }
	//コイン
	for (int i = 0; i <= FEET_START_NUM; i++) { AddGameObject<Feet>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));}
	for (int i = 0; i < SEED_START_NUM; i++) {AddGameObject<Seed>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2));}
	//岩
	for (int i = 0; i < ROCK_START_NUM; i++) { AddGameObject<Rock>(1)->SetPosition(D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2)); }

	AddGameObject<MouseIcon>(3);				//マウスアイコン
	m_TimeFade = AddGameObject<TimeFade>(2);	//時間帯の色
	AddGameObject<DayBar>(2);					//時間帯の背景
	AddGameObject<HpBar>(2);					//プレイヤーのHPバー
	AddGameObject<Score>(2);					//スコア表示
	m_Fade = AddGameObject<Fade>(3);			//シンプルなフェード

	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\chiisanaashiato.wav");
	m_BGM->Play(0.0f,true);
	m_BGM_Night = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM_Night->Load("asset\\audio\\latenightsnow.wav");
	m_BGM_Night->Play(0.0f, true);
	m_SE_Whistle = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_Whistle->Load("asset\\audio\\警官のホイッスル1.wav");
	m_SE_SheepCry = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_SheepCry->Load("asset\\audio\\ヒツジの鳴き声.wav");
	m_SE_Bell = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_Bell->Load("asset\\audio\\鈴を鳴らす.wav");
	m_SE_Gong = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_Gong->Load("asset\\audio\\魔の時計塔の鐘.wav");
	m_SE_WolfCry = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE_WolfCry->Load("asset\\audio\\オオカミの遠吠え.wav");
}

void Game::Uninit()
{
	Scene::Uninit();
	Apple::Unload();
	Box::Unload();
	Cylinder::Unload();
	Spring::Unload();
	Tree::Unload();
	Follow::Unload();
	Human::Unload();
	HumanSpring::Unload();
	Goal::Unload();
	GoalScope::Unload();
	GoalNavigation::Unload();
	Player::Unload();
	Seed::Unload();
	Wolf::Unload();
	DisasterWolf::Unload();
	Explosion::Unload();
	Smoke::Unload();
	AngelRing::Unload();
	Shine::Unload();
	Shine2::Unload();
	GoalText::Unload();
	FeetEffect::Unload();
	HpBarS::Unload();
	Feet::Unload();
	BreakObject::Unload();
	Chest::Unload();
	SheepCage::Unload();
	Rock::Unload();
	Fence::Unload();
	ItemSpeed::Unload();
	ItemEye::Unload();
	ItemLife::Unload();
	DamageFade::Unload();
	Shadow::Unload();
	AttackMarker::Unload();
	TimeFade::Unload();
	ConcentrationLine::Unload();
	DayBar::Unload();
}

void Game::Update()
{
	Scene::Update();

	//BGMのフェード更新
	m_BGM->FadeUpdate();
	m_BGM_Night->FadeUpdate();

	switch (m_GameState)
	{
	case GAME_STATE::START:
		UpdateStart();
		break;
	case GAME_STATE::NORMAL:
		UpdateNormal();
		break;
	case GAME_STATE::CLEAR:
		UpdateClear();
		break;
	case GAME_STATE::FADE:
		UpdateFade();
		break;
	}
}

void Game::UpdateStart()
{
	//ゲーム開始時の演出
	if (m_StartTextTime == 0)
	{
		AddGameObject<InfoLog>(2)->SetNum(14, 5, D3DXVECTOR3(220, -30, 0));
	}
	else if (m_StartTextTime == 100)
	{
		m_SE_Bell->Play(1.0f, false);
		AddGameObject<InfoLog>(2)->SetNum(20, 4, D3DXVECTOR3(340, 60, 0));
	}
	else if (m_StartTextTime == 150)
	{
		m_SE_Bell->Play(1.0f, false);
		AddGameObject<InfoLog>(2)->SetNum(21, 4, D3DXVECTOR3(340, 60, 0));
	}
	else if (m_StartTextTime == 200)
	{
		m_SE_Bell->Play(1.0f, false);
		AddGameObject<InfoLog>(2)->SetNum(22, 4, D3DXVECTOR3(340, 60, 0));
	}
	else if (m_StartTextTime >= 250)
	{
		m_SE_Whistle->Play(1.0f, false);
		AddGameObject<InfoLog>(2)->SetNum(23, 4, D3DXVECTOR3(340, 60, 0));
		m_BGM->Play(1.0f, true);
		m_GameState = GAME_STATE::NORMAL;
		AddGameObject<InfoLogTutorial>(2);
	}
	m_StartTextTime++;

}

void Game::UpdateNormal()
{
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	//秒数更新
	m_GameTime++;
	if (m_GameTime > 60)
	{
		TimeSecondsUpdate();
		m_GameTime = 0;
	}

	//秒毎のイベント
	if (m_GameTime == 0)
	{
		if (m_GameTimeSeconds == 30) { TimeEvent_Time30(); }										//30秒に1回
		else if (m_GameTimeSeconds == 60) { TimeEvent_Time60(); }									//60秒に1回
		else if (m_GameTimeSeconds == 15 || m_GameTimeSeconds == 45) { TimeEvent_Time15or45(); }	//15か45秒に1回
		if (m_GameTimeSeconds % 2 == 0) { TimeEvent_Time2Loop(); }									//2秒ごと
	}

	//災害狼の発生数を調整する処理
	DisasterWolfEmitter();

	//プレイヤーが食べられたらゲームオーバー
	Player* player = scene->GetGameObject<Player>();
	if (player->GetCharacterState() == CHARACTER_STATE::UNUSED)
	{
		m_BGM->FadeToVolume(0.0f, 0.01f);
		m_BGM_Night->FadeToVolume(0.0f, 0.01f);
		m_GameState = GAME_STATE::FADE;
	}

	//仲間が100匹になったらゲームクリア
	if (score->GetCount() >= 100) { m_GameState = GAME_STATE::CLEAR; }

	//デバッグモード専用処理
	bool debug = player->GetDebug();
	if (Input::GetKeyPress(VK_RBUTTON) && debug)	//羊増量
	{
		D3DXVECTOR3 PLPos = player->GetPosition();
		AddGameObject<Follow>(1)->SetPosition(D3DXVECTOR3(PLPos.x, 1.0f, PLPos.z));
	}	
}

void Game::UpdateClear()
{
	//100匹集まってステージクリアしたときの処理
	Scene* scene = Manager::GetScene();

	m_ClearTime++;
	if (m_ClearTime == 50)
	{
		m_SE_Bell->Play(1.0f, false);
		AddGameObject<InfoLog>(2)->SetNum(20, 4, D3DXVECTOR3(340, 0, 0));
	}
	else if (m_ClearTime == 100)
	{
		m_SE_Bell->Play(1.0f, false);
		AddGameObject<InfoLog>(2)->SetNum(21, 4, D3DXVECTOR3(340, 0, 0));
	}
	else if (m_ClearTime == 150)
	{
		m_SE_Bell->Play(1.0f, false);
		AddGameObject<InfoLog>(2)->SetNum(22, 4, D3DXVECTOR3(340, 0, 0));
	}
	else if (m_ClearTime == 200)
	{
		auto wolfs = scene->GetGameObjects<Wolf>();
		for (Wolf* wolf : wolfs)
		{
			wolf->SetDaathTimeDelete();
		}

		AddGameObject<InfoLog>(2)->SetNum(24, 4, D3DXVECTOR3(340, 0, 0));
		m_SE_Whistle->Play(1.0f, false);
		m_BGM->FadeToVolume(0.0f, 1.0f);
		m_BGM_Night->FadeToVolume(0.0f, 1.0f);
	}
	else if (m_ClearTime >= 250) { m_GameState = GAME_STATE::FADE; }
	
	//羊の数が足りなければゲームに戻る
	Score* score = scene->GetGameObject<Score>();
	if (score->GetCount() < 100) 
	{ 
		m_ClearTime = 0;
		m_GameState = GAME_STATE::NORMAL; 
	}
}

void Game::UpdateFade()
{
	m_Fade->FadeOut(); 

	//フェードが終わったら
	if (m_Fade->GetFadeFinish())
	{
		//リザルトに情報をセットする
		Scene* scene = Manager::GetScene();
		Score* score = scene->GetGameObject<Score>();
		Result* result = scene->GetGameObject<Result>();
		result->SetDay(score->GetCountDay());
		result->SetCoin(score->GetCountCoin());
		result->SetCount(score->GetFullCount());

		Manager::SetScene<Result>();
	}
}

void Game::TimeSecondsUpdate()
{
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	m_GameTimeSeconds ++;

	//時間データを渡す
	DayBar* daybar = scene->GetGameObject<DayBar>();
	daybar->SetTimeFC(60, m_GameTimeSeconds);

	//時限イベント終了
	m_NawEvent --;
	if (m_NawEvent <= 0) 
	{
		m_NawEvent = 0;
		if (m_Event1 || m_Event2) {
			m_Event1 = false;
			m_Event2 = false;
			m_SE_SheepCry->Play(1.0f, false);
			AddGameObject<InfoLog>(2)->SetNum(9, 3, D3DXVECTOR3(900, 480, 0));
		}
	}
}

void Game::TimeEvent_Time2Loop()
{
	//2秒毎のイベント
	//読み込み処理
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	D3DXVECTOR3 PLPos = player->GetPosition();
	Score* score = scene->GetGameObject<Score>();
	int count = score->GetCount();

	int enemyStatusCount = count / 20 + 1;  //敵ステータス計算処理

	//敵生成(仲間の数で変動)
	if (count >= 0)
	{
		float rot = frand() * 2 * D3DX_PI;
		AddGameObject<Wolf>(1)->SetPosEnemyData(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z), enemyStatusCount + 1);

		if (irand(1, 2) == 1)
		{
			rot = frand() * 2 * D3DX_PI;
			AddGameObject<Human>(1)->SetPosition(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z));
		}
	}
	if (count >= 10)
	{
		for (int i = 1; i <= enemyStatusCount / 3; i++) 
		{
			int minus = 0;
			float rot = frand() * 2 * D3DX_PI;
			if (irand(1, 3) == 1) { minus = -1; }

			AddGameObject<Wolf>(1)->SetPosEnemyData(D3DXVECTOR3(static_cast<float>((cos(rot))) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>((sin(rot))) * ENEMY_MAKE_DISTANCE + PLPos.z), enemyStatusCount + minus);
		}
	}

	if (irand(1, 3) == 1 && count >= 10)
	{
		float rot = frand() * 2 * D3DX_PI;
		AddGameObject<HumanSpring>(1)->SetPosition(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z));
	}

	//コイン(毎回7枚)
	for (int i = 0; i <= 7; i++) {AddGameObject<Feet>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));}
	
	//仲間
	if (irand(1, 3) <= 2) { AddGameObject<Follow>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));}

	//イベントフラグで仲間追加生成
	if (m_Event1)
	{
		AddGameObject<Follow>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));
	}

	//羊の檻
	if (irand(1, 3) == 1) { AddGameObject<SheepCage>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2)); }

	//チェスト
	if (irand(1, 5) == 1) { AddGameObject<Chest>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));}
	
	//木
	if (irand(1, 3) <= 2) { AddGameObject<Tree>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2)); }

	//岩
	if (irand(1, 3) == 1) { AddGameObject<Rock>(1)->SetPosition(D3DXVECTOR3(PLPos.x + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2, 0.0f, PLPos.z + frand() * STAGE_MAKE_PL_XY - STAGE_MAKE_PL_XY / 2));}

}

void Game::TimeEvent_Time30()
{
	//夜にする
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

void Game::TimeEvent_Time15or45()
{
	//イベント発生用
	if (m_NawEvent == 0) 
	{
		if (m_Day >= 3) //3日目から12分の1の確率で発生する
		{
			if (irand(1, 12) == 1)
			{
				int randam = irand(1, 2);
				if (randam == 1)
				{
					//羊の群れ
					AddGameObject<InfoLog>(2)->SetNum(7, 1, D3DXVECTOR3(340, 0, 0));
					m_SE_Bell->Play(1.0f, false);
					m_Event1 = true;
				}
				else if (randam == 2)
				{
					//狼が来た
					AddGameObject<InfoLog>(2)->SetNum(8, 1, D3DXVECTOR3(340, 0, 0));
					m_SE_Gong->Play(1.0f, false);
					m_Event2 = true;
				}
				m_NawEvent = 60;//40秒後まで再発生しない

			}
		}
	}
}

void Game::TimeEvent_Time60()
{
	//朝にする
	m_Day++;
	if (m_Day > 99) { m_Day = 99; }

	m_TimeFade->FadeOut_false();
	m_GameTimeSeconds = 0;
	AddGameObject<InfoLog>(2)->SetNum(1, 1, D3DXVECTOR3(340, 0, 0));
	AddGameObject<InfoLog>(2)->SetNum(6, 2, D3DXVECTOR3(340, 640, 10));

	m_SE_SheepCry->Play(1.0f, false);
	m_SE_Bell->Play(1.0f, false);
	m_BGM->FadeToVolume(1.0f, 0.002f);
	m_BGM_Night->FadeToVolume(0.0f, 0.004f);

	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();
	score->SetCountDay(m_Day);
	score->SetTimeZone(false);
}

void Game::DisasterWolfEmitter()
{
	//災害狼の出現処理
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	//災害狼の数の増減
	int count = score->GetCount();
	int disasterCount = 0;
	int lowCount = 0;
	// countで作る数変動
	if (count >= 15) { disasterCount++; }
	if (count >= 40) { disasterCount++; }
	if (count >= 85) { disasterCount++; }

	// 使われている数を判断
	auto disasterWolfs = scene->GetGameObjects<DisasterWolf>();
	// 不足している場合、生成
	while (disasterWolfs.size() < disasterCount)
	{
		Player* player = scene->GetGameObject<Player>();
		D3DXVECTOR3 PLPos = player->GetPosition();
		float rot = frand() * 2 * D3DX_PI;
		DisasterWolf* disasterWolf = AddGameObject<DisasterWolf>(1);
		disasterWolf->SetPosEnemyData(D3DXVECTOR3(static_cast<float>(cos(rot)) * ENEMY_MAKE_DISTANCE + PLPos.x, 0.0f, static_cast<float>(sin(rot)) * ENEMY_MAKE_DISTANCE + PLPos.z), disasterCount);
		// ウルフのポインタをリストに追加
		disasterWolfs.push_back(disasterWolf);
	}
	// 過剰な場合、削除
	while (disasterWolfs.size() > disasterCount)
	{
		DisasterWolf* wolfToRemove = disasterWolfs.back();
		wolfToRemove->SetDaathTimeDelete();
		disasterWolfs.pop_back();
	}
}
