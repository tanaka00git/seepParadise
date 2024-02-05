#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\wolf.h"
#include "..\GameObject\disasterWolf.h"
#include "..\App\model.h"

Model* DisasterWolf::m_ModelClown{};

void DisasterWolf::Load()
{
	m_ModelClown = new Model();
	m_ModelClown->Load("asset\\model\\wolfClown1.obj");
}

void DisasterWolf::Unload()
{
	m_ModelClown->Unload();
	delete m_ModelClown;
	m_ModelClown = nullptr;
}

void DisasterWolf::DisasterMove()
{
	//自動消滅しないように適当な値で上書き
	m_DaathTime = 1000;

	//一定時間経つとオートで敵を生み出す
	Scene* scene = Manager::GetScene();
	m_DisasterCount++;
	if (m_DisasterCount >= 150)
	{
		Wolf* wolf = scene->AddGameObject<Wolf>(1);
		wolf->SetEnemyData(1);
		wolf->SetPosition(m_Position);
		m_DisasterCount = 0;
	}
}

void DisasterWolf::UpdateTargeting()
{
	Wolf::UpdateTargeting();

	//災害狼専用イベント
	DisasterMove();
}

void DisasterWolf::Draw()
{
	Wolf::Draw();
	m_ModelClown->DrawColor(m_Color, m_TextureEnable); 
}

void DisasterWolf::SetEnemyData(int data)
{
	m_Item = false;
	m_BiteCount = 2;
	m_FullLife = 80;
	m_Speed = 0.04f;
	m_CoinDrop = 30;
	m_StanGuard = 40;
	m_OriginalScale = D3DXVECTOR3(2.9f, 2.9f, 2.9f);	//キャラのサイズ
	m_BarScale = D3DXVECTOR3(5.0f, 1.0f, 1.0f);			//HPバーのサイズ
	m_Tracking = 200.0f;								//追尾範囲
	m_Life = m_FullLife;
}
