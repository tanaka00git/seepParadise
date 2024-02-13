#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\App\input.h"
#include "..\Scene\result.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\mouseIcon.h"
#include "..\GameObject\resultSheep.h"
#include "..\GameObject\resultCoin.h"
#include "..\GameObject\resultLogo.h"
#include "..\GameObject\resultScore.h"
#include "..\GameObject\resultSky.h"
#include "..\Scene\title.h"
#include "..\GameObject\fade.h"
#include "..\App\audio.h"

int Result::m_Day{};
int Result::m_Coin{};
int Result::m_Count{};

void Result::Init()
{
	ResultSheep::Load();
	ResultCoin::Load();
	ResultSky::Load();

	Camera*camera = AddGameObject<Camera>(0);
	camera->SetCameraState(CAMERA_STATE::RESULT);

	AddGameObject<MouseIcon>(3);
	AddGameObject<ResultLogo>(2);
	ResultScore*resultScore = AddGameObject<ResultScore>(2);
	resultScore->SetCount(m_Count);
	resultScore->SetCountCoin(m_Coin);
	resultScore->SetCountDay(m_Day);

	//空(ゲームで仲間になった数だけ明るくなる)
	ResultSky* resultSky = AddGameObject<ResultSky>(0);
	resultSky->SetCountLight(m_Count);

	//羊(ゲームで仲間になっただけ増える)
	for (int i = 1; i <= m_Count; i++) {
		ResultSheep*resultSheep = AddGameObject<ResultSheep>(1);

		resultSheep->SetPosition(D3DXVECTOR3(0.0f, 3.0f + i * 10, 0.0f));
		resultSheep->SetRotation(D3DXVECTOR3(0.0f, frand()* 40.0f, 0.0f));
	}
	//コイン(入手しただけ増える)
	for (int i = 1; i <= m_Coin / 10; i++)
	{
		AddGameObject<ResultCoin>(1)->SetPosition(D3DXVECTOR3(0.0f, 3.0f + i * 3, 0.0f));
	}

	m_Fade = AddGameObject<Fade>(2);

	m_BGM = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_BGM->Load("asset\\audio\\yuuguredoki.wav");
	m_BGM->Play(1.0f,true);
	m_SE = AddGameObject<GameObject>(0)->AddComponent<Audio>();
	m_SE->Load("asset\\audio\\決定ボタンを押す28.wav");
}

void Result::Uninit()
{
	Scene::Uninit();
	ResultSheep::Unload();
	ResultCoin::Unload();
	ResultSky::Unload();
}

void Result::Update()
{
	Scene::Update();

	switch (m_ResultState)
	{
	case RESULT_STATE::NORMAL:
		UpdateNormal();
		break;
	case RESULT_STATE::FADE:
		UpdateFade();
		break;
	}

}

void Result::UpdateNormal()
{
	//キー入力でフェード
	if (Input::GetKeyTrigger(VK_LBUTTON))
	{
		m_SE->Play(1.0f);
		m_Fade->FadeOut();
		m_ResultState = RESULT_STATE::FADE;
	}
}

void Result::UpdateFade()
{
	if (m_Fade->GetFadeFinish())
	{
		Scene* scene = Manager::GetScene();
		Title* title = scene->GetGameObject<Title>();
		title->AddCoin(m_Coin);
		Manager::SetScene<Title>();
	}
}
