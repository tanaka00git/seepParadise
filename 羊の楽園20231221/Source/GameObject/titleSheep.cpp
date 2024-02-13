#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\titleSheep.h"
#include "..\App\model.h"

Model* TitleSheep::m_Model{};

void TitleSheep::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sheepMini.obj");
}

void TitleSheep::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void TitleSheep::Init()
{
	m_Position.x = (frand() * 20.0f) - 10.0f;
	m_Position.z = 3.0f;
	m_Position.y = 4.5f;
	m_Scale = D3DXVECTOR3(0.4f, 0.4f, 0.4f);
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void TitleSheep::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void TitleSheep::Update()
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


void TitleSheep::Draw()
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
