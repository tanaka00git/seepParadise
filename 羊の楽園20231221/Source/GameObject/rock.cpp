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
	GameObject::Draw();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->DrawColor(m_Color, m_TextureEnable);
}

void Rock::UpdateDead()
{
	BreakObject::UpdateDead();

	//Init����
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//�A�C�e���h���b�v
		int a = irand(1, DROP_RAIT);
		if (a == 1)
		{
			//�ǂꂩ���I�o�����
			int b = irand(1, 3);
			// �X�s�[�h�A�b�v
			if (b == 1)
			{
				ItemSpeed* itemSpeed = scene->AddGameObject<ItemSpeed>(1);
				itemSpeed->SetPosition(m_Position);
				itemSpeed->SetDrop();
			}
			// ���C�t�A�b�v
			else if (b == 2)
			{
				ItemLife* itemLife = scene->AddGameObject<ItemLife>(1);
				itemLife->SetPosition(m_Position);
				itemLife->SetDrop();
			}
			// ����A�b�v
			else if (b == 3)
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

