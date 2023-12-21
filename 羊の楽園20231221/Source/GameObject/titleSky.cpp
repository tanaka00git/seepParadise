#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\App\manager.h"
#include "..\Scene\scene.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\titleSky.h"
#include "..\App\model.h"

Model*TitleSky::m_Model{};

void TitleSky::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\skydm01.obj");//mtlのkdを1にすること！
}

void TitleSky::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void TitleSky::Init()
{
	m_Scale = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
	m_Position = D3DXVECTOR3(0.0f, -20.0f, 0.0f);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void TitleSky::Uninit()
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void TitleSky::Update()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject <Camera> ();

	m_Position.x = camera->GetPosition().x;
	m_Position.z = camera->GetPosition().z;

	m_Rotation.y -= 0.001f;
}

void TitleSky::Draw()
{
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