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

#define COIN_DROP 3
#define ORIGINAL_SCALE_X 3.0f

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

	m_Scale = D3DXVECTOR3(ORIGINAL_SCALE_X, 0.0f, 0.0f);
	m_OriginalScale = D3DXVECTOR3(ORIGINAL_SCALE_X, 1.0f, 1.0f);
}

void Fence::Draw()
{
	GameObject::Draw();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}

void Fence::UpdateDead()
{
	BreakObject::UpdateDead();

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

