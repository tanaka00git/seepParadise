#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\tree.h"
#include "..\GameObject\shadow.h"
#include "..\App\model.h"
#include "..\Scene\scene.h"
#include "..\App\manager.h"
#include "..\GameObject\camera.h"

Model*Tree::m_Model{};

void Tree::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\tree_a.obj");
}

void Tree::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Tree::Init()
{
	m_Scale.y = 0.01f;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_Shadow = AddComponent<Shadow>();

}

void Tree::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void Tree::Update()
{
	GameObject::Update();

	if (m_Scale.y < 1.0f) {m_Scale.y += 0.05f;}
	else { m_Scale.y = 1.0f;}

	//影の移動
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = m_Position.y + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
}


void Tree::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera->CheckView(m_Position, m_Scale)) { return; }

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