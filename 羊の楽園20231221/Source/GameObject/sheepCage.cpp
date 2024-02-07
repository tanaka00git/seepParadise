#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\sheepCage.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\follow.h"
#include "..\GameObject\hpBarS.h"
#include "..\App\audio.h"
#include "..\App\model.h"

Model* SheepCage::m_ModelCage{};
Model* SheepCage::m_ModelCageSheep{};
Audio* SheepCage::m_SE_Chest{};

#define SHEEP_ROTATION_Y 0.01f
#define SHEEP_DROP 3
#define HB_BAR_POS_Y 2.3f

void SheepCage::Load()
{
	m_ModelCage = new Model();
	m_ModelCage->Load("asset\\model\\cage.obj");
	m_ModelCageSheep = new Model();
	m_ModelCageSheep->Load("asset\\model\\CageSheep.obj");
	m_SE_Chest = new Audio();
	m_SE_Chest->Load("asset\\audio\\ドアを開ける2.wav");
}

void SheepCage::Unload()
{
	m_ModelCage->Unload();
	delete m_ModelCage;
	m_ModelCageSheep->Unload();
	delete m_ModelCageSheep;
}

void SheepCage::Init()
{
	m_HpBarPosY = HB_BAR_POS_Y;	//HPバーのY座標
	BreakObject::Init();
}

void SheepCage::Draw()
{
	GameObject::Draw();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x * 1.0f, m_Scale.y * 1.0f, m_Scale.z * 1.0f);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);
	m_ModelCage->DrawColor(m_Color, m_TextureEnable);

	//羊描画
	m_SheepRotationY += SHEEP_ROTATION_Y;
	D3DXMatrixRotationYawPitchRoll(&rot, m_SheepRotationY, m_Rotation.x, m_Rotation.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);
	m_ModelCageSheep->DrawColor(m_Color, m_TextureEnable);
}

void SheepCage::UpdateDead()
{
	BreakObject::UpdateDead();

	//Init処理
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_HpBarS->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_SE_Chest->Play(1.0f);

		//コインドロップ
		for (int i = 0; i <= SHEEP_DROP; i++)
		{
			Follow* follow = scene->AddGameObject<Follow>(1);
			follow->SetPosition(m_Position);
			follow->SetDrop();
		}
	}
}

