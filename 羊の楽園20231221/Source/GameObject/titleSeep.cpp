#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\titleSeep.h"
#include "..\App\model.h"

Model* TitleSeep::m_Model{};

void TitleSeep::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\seep_a.obj");
}

void TitleSeep::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void TitleSeep::Init()
{
	m_Position.x = (frand() * 20.0f) - 10.0f;
	m_Position.z = 3.0f;
	m_Position.y = 4.5f;
	m_Scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void TitleSeep::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void TitleSeep::Update()
{
	m_Rotation.y -= 0.05f;
	m_Position.y -= 0.008f;
	m_Scale.x -= 0.001f;
	m_Scale.y -= 0.001f;
	m_Scale.z -= 0.001f;

	if (m_Scale.x <= 0.0f)
	{
		SetDestroy();
	}
}


void TitleSeep::Draw()
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
