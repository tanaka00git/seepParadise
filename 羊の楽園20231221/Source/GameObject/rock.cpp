#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\breakObject.h"
#include "..\App\audio.h"
#include "..\GameObject\coin.h"
#include "..\GameObject\itemSpeed.h"
#include "..\GameObject\itemLife.h"
#include "..\GameObject\itemEye.h"
#include "..\GameObject\hpBarS.h"
#include "..\App\model.h"

Model*Rock::m_Model{};

#define DROP_RAIT 12
#define COIN_DROP 4

void Rock::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\rock.obj");
}

void Rock::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Rock::Draw()
{
	BreakObject::Draw();
	m_Model->Draw();
}

void Rock::UpdateDeath()
{
	BreakObject::UpdateDeath();

	//Init����
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//�A�C�e���h���b�v
		int a = irand(0, DROP_RAIT-1);
		if (a == 0)
		{
			//�ǂꂩ���I�o�����
			int b = irand(0, 2);
			// �X�s�[�h�A�b�v
			if (b == 0)
			{
				ItemSpeed* itemSpeed = scene->AddGameObject<ItemSpeed>(1);
				itemSpeed->SetPosition(m_Position);
				itemSpeed->SetDrop();
			}
			// ���C�t�A�b�v
			else if (b == 1)
			{
				ItemLife* itemLife = scene->AddGameObject<ItemLife>(1);
				itemLife->SetPosition(m_Position);
				itemLife->SetDrop();
			}
			// ����A�b�v
			else if (b == 2)
			{
				ItemEye* itemEye = scene->AddGameObject<ItemEye>(1);
				itemEye->SetPosition(m_Position);
				itemEye->SetDrop();
			}
		}

		//�R�C���h���b�v
		for (int i = 0; i <= COIN_DROP; i++)
		{
			Coin* coin = scene->AddGameObject<Coin>(1);
			coin->SetPosition(m_Position);
			coin->SetDrop();
		}
	}
}

