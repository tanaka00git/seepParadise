#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\fence.h"
#include "..\GameObject\breakObject.h"
#include "..\App\audio.h"
#include "..\GameObject\hpBarS.h"
#include "..\App\model.h"

Model* Fence::m_Model{};

const float ORIGINAL_SCALE_X = 4.0f;
const int INITIAL_LIFE = 16;

void Fence::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\fence.obj");
}

void Fence::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Fence::Init()
{
	BreakObject::Init();

	m_Life = INITIAL_LIFE;
	m_FullLife = INITIAL_LIFE;
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

	m_Model->DrawColor(m_Color, m_TextureEnable);
}

void Fence::UpdateDead()
{
	BreakObject::UpdateDead();

	//Init処理
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_HpBarS->SetDraw(false);
	}
}

