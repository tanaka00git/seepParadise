#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\seed.h"
#include "..\Scene\scene.h"
#include "..\App\manager.h"
#include "..\GameObject\camera.h"
#include "..\App\model.h"

Model*Seed::m_Model{};

#define ORIGINAL_SCALSE D3DXVECTOR3(2.0f, 2.0f, 2.0f)

void Seed::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\seed.obj");
}

void Seed::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void Seed::Init()
{
	m_OriginalScale = ORIGINAL_SCALSE;
	m_Scale.y = 0.01f;
	m_Scale.x = m_OriginalScale.x;
	m_Scale.z = m_OriginalScale.z;


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void Seed::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Seed::Update()
{
	if (m_Scale.y < m_OriginalScale.y) { m_Scale.y += 0.05f; }
	else { m_Scale.y = m_OriginalScale.y; }
}

void Seed::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera->CheckView(m_Position, m_Scale)) { return; }

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x / 2, m_Scale.y / 2, m_Scale.z / 2);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();
}