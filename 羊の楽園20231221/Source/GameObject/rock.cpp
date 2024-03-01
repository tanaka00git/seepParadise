#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\rock.h"
#include "..\GameObject\breakObject.h"
#include "..\App\audio.h"
#include "..\GameObject\feet.h"
#include "..\GameObject\hpBarS.h"
#include "..\App\model.h"

Model*Rock::m_Model{};

#define FEET_DROP 3

void Rock::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\rock.obj");
}

void Rock::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Rock::Draw()
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

	m_Model->DrawColor(m_Color, m_TextureEnable);
}

void Rock::UpdateDead()
{
	BreakObject::UpdateDead();

	//Init処理
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_HpBarS->SetDraw(false);

		//コインドロップ
		for (int i = 0; i <= FEET_DROP; i++)
		{
			Feet* feet = scene->AddGameObject<Feet>(1);
			feet->SetPosition(m_Position);
			feet->SetDrop();
		}
	}
}

