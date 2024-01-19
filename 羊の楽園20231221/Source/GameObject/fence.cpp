#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\fence.h"
#include "..\GameObject\breakObject.h"
#include "..\App\audio.h"
#include "..\GameObject\coin.h"
#include "..\GameObject\hpBarS.h"
#include "..\App\model.h"

Model* Fence::m_Model{};

#define DROP_RAIT 12
#define COIN_DROP 3

void Fence::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\fence.obj");
}

void Fence::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Fence::Init()
{
	BreakObject::Init();

	int a = irand(0, 2);
	if (a == 1)
	{
		m_Rotation.y = 3.14f / 2;
	}

	m_Scale = D3DXVECTOR3(4.0f, 0.0f, 0.0f);
	m_OriginalScale = D3DXVECTOR3(4.0f, 1.0f, 1.0f);
}

void Fence::Draw()
{
	BreakObject::Draw();
	m_Model->Draw();
}

void Fence::UpdateDeath()
{
	BreakObject::UpdateDeath();

	//Init処理
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//コインドロップ
		for (int i = 0; i <= COIN_DROP; i++)
		{
			Coin* coin = scene->AddGameObject<Coin>(1);
			coin->SetPosition(m_Position);
			coin->SetDrop();
		}
	}
}

