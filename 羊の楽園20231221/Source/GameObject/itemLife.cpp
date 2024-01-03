#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\itemLife.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\player.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\infoLog.h"
#include "..\App\model.h"

Model*ItemLife::m_Model{};
Audio*ItemLife::m_SE_PowerUp{};
Audio*ItemLife::m_SE_Get{};

void ItemLife::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\item2.obj");

	m_SE_PowerUp = new Audio();
	m_SE_PowerUp->Load("asset\\audio\\�X�e�[�^�X�㏸.wav");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\�ς���.wav");
}

void ItemLife::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void ItemLife::Draw()
{
	ItemObject::Draw();
	m_Model->Draw();
}

void ItemLife::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(11, 3, D3DXVECTOR3(900, 480, 0));			//�C���t�H����
	DamageFade* damageFade = scene->AddGameObject<DamageFade>(2);
	damageFade->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	player->AddFullLife(+1);
	player->AddLife(+1);

	m_SE_PowerUp->Play(1.0f);
	m_SE_Get->Play(1.0f);
}
