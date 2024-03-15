#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\titleBox.h"
#include "..\App\model.h"

Model*TitleBox::m_Model{};

const float INIT_ROTATION_Y = -2.3f;
const float INIT_POSITION_Y = 1.0f;
const float SCALE = 0.8f;
const float ROTATION_Y_SPEED = -0.004f;

void TitleBox::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\boxsupar-9.obj");
}

void TitleBox::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void TitleBox::Init()
{
	m_Rotation.y = INIT_ROTATION_Y;
	m_Position.y = INIT_POSITION_Y;
	m_Scale = D3DXVECTOR3(SCALE, SCALE, SCALE);
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void TitleBox::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}


void TitleBox::Update()
{
	m_Rotation.y += ROTATION_Y_SPEED;
}

void TitleBox::Draw()
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