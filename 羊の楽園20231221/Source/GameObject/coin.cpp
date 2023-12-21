#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\coin.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\score.h"
#include "..\GameObject\player.h"
#include "..\GameObject\infoLog.h"
#include "..\App\model.h"

Model*Coin::m_Model{};
Audio*Coin::m_SE_Get{};

void Coin::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\coin.obj");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\d‰ÝE“n‚·02b.wav");

}

void Coin::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Coin::Draw()
{
	ItemObject::Draw();
	m_Model->Draw();
}

void Coin::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(3, 2, D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	score->AddCountCoin(+1);

	Player* player = scene->GetGameObject<Player>();
	player->AddCharge(+300);

	m_SE_Get->Play(1.0f);
}