#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\Scene\scene.h"
#include "..\App\renderer.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\camera.h"
#include "..\App\model.h"

Model*Cylinder::m_Model{};

void Cylinder::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\cylinder.obj");
}

void Cylinder::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Cylinder::Init()
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void Cylinder::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Cylinder::Update()
{
	GameObject::Update();
}

void Cylinder::Draw()
{
	GameObject::Draw();

	//視錐台カリング
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera->CheckView(m_Position, m_Scale)) { return; }

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x * 1.2f, m_Scale.y * 0.7f, m_Scale.z * 1.2f);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);
	m_Model->Draw();
}