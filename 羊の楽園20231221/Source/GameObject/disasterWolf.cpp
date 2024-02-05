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
	//�������ł��Ȃ��悤�ɓK���Ȓl�ŏ㏑��
	m_DaathTime = 1000;

	//��莞�Ԍo�ƃI�[�g�œG�𐶂ݏo��
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

	//�ЊQ�T��p�C�x���g
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
	m_OriginalScale = D3DXVECTOR3(2.9f, 2.9f, 2.9f);	//�L�����̃T�C�Y
	m_BarScale = D3DXVECTOR3(5.0f, 1.0f, 1.0f);			//HP�o�[�̃T�C�Y
	m_Tracking = 200.0f;								//�ǔ��͈�
	m_Life = m_FullLife;
}
