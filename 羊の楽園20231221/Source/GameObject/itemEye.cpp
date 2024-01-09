#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\itemEye.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\player.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\infoLog.h"
#include "..\GameObject\shine2.h"
#include "..\App\model.h"

Model*ItemEye::m_Model{};
Audio*ItemEye::m_SE_PowerUp{};
Audio*ItemEye::m_SE_Get{};

void ItemEye::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\item3.obj");

	m_SE_PowerUp = new Audio();
	m_SE_PowerUp->Load("asset\\audio\\ステータス上昇.wav");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\ぱくっ.wav");
}

void ItemEye::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void ItemEye::Draw()
{
	ItemObject::Draw();
	m_Model->Draw();
}

void ItemEye::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(12, 3, D3DXVECTOR3(900, 480, 0));			//インフォ発生
	DamageFade* damageFade = scene->AddGameObject<DamageFade>(2);
	damageFade->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	player->AddEye(+1);

	m_SE_PowerUp->Play(1.0f);
	m_SE_Get->Play(1.0f);

	scene->AddGameObject<Shine2>(1)->SetPosition(m_Position);//エフェクト
}