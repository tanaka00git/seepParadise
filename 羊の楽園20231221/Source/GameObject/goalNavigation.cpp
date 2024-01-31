#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\goalNavigation.h"
#include "..\App\model.h"
#include "..\Scene\scene.h"
#include "..\App\manager.h"
#include "..\GameObject\camera.h"

#define SCALE_Y 0.05f

Model* GoalNavigation::m_Model{};

void GoalNavigation::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\targetNavi.obj");
}

void GoalNavigation::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void GoalNavigation::Init()
{
	m_Scale.y = SCALE_Y;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void GoalNavigation::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void GoalNavigation::Update()
{
	GameObject::Update();
}


void GoalNavigation::Draw()
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