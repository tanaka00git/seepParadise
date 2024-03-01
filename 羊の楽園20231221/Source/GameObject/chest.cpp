#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\feet.h"
#include "..\GameObject\shine.h"
#include "..\GameObject\itemSpeed.h"
#include "..\GameObject\itemLife.h"
#include "..\GameObject\itemEye.h"
#include "..\GameObject\hpBarS.h"
#include "..\App\audio.h"
#include "..\App\model.h"

Model*Chest::m_Model{};
Audio*Chest::m_SE_Chest{};

#define DROP_RAIT 3
#define FEET_DROP 9

void Chest::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\chest.obj");
	m_SE_Chest = new Audio();
	m_SE_Chest->Load("asset\\audio\\�h�A���J����2.wav");
}

void Chest::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Chest::Draw()
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

void Chest::UpdateDead()
{
	BreakObject::UpdateDead();

	//Init����
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_SE_Chest->Play(1.0f);

		//�A�C�e���h���b�v
		if (irand(1, DROP_RAIT) == 1)
		{	
			//�ǂꂩ���I�o�����
			int random = irand(1, 3);
			// �X�s�[�h�A�b�v
			if (random == 1)
			{
				ItemSpeed* itemSpeed = scene->AddGameObject<ItemSpeed>(1);
				itemSpeed->SetPosition(m_Position);
				itemSpeed->SetDrop();
			}
			// ���C�t�A�b�v
			else if (random == 2)
			{
				ItemLife* itemLife = scene->AddGameObject<ItemLife>(1);
				itemLife->SetPosition(m_Position);
				itemLife->SetDrop();
			}
			// ����A�b�v
			else if (random == 3)
			{
				ItemEye* itemEye = scene->AddGameObject<ItemEye>(1);
				itemEye->SetPosition(m_Position);
				itemEye->SetDrop();
			}
		}

		//�R�C���h���b�v
		for (int i = 0; i <= FEET_DROP; i++)
		{
			Feet* feet = scene->AddGameObject<Feet>(1);
			feet->SetPosition(m_Position);
			feet->SetDrop();
		}
		scene->AddGameObject<Shine>(1)->SetPosition(m_Position);//�R�C���G�t�F�N�g

	}
}

