#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\apple.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\score.h"
#include "..\GameObject\player.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\infoLog.h"
#include "..\App\model.h"

Model*Apple::m_Model{};
Audio*Apple::m_SE_Get{};
Audio*Apple::m_SE_Heel{};

void Apple::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\apl.obj");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\‚Ï‚­‚Á.wav");
	m_SE_Heel = new Audio();
	m_SE_Heel->Load("asset\\audio\\ƒVƒƒƒ‰ƒ“ƒb.wav");
}

void Apple::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Apple::Draw()
{
	ItemObject::Draw();
	m_Model->Draw();
}

void Apple::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(5, 2, D3DXVECTOR3(100, -10, 0));
	DamageFade* damageFade = scene->AddGameObject<DamageFade>(2);
	damageFade->SetColorData(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f));
	player->AddLife(+1);

	m_SE_Get->Play(1.0f);
	m_SE_Heel->Play(1.0f);
}
