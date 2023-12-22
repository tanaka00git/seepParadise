#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\resultCoin.h"
#include "..\GameObject\shadow.h"
#include "..\App\model.h"

Model*ResultCoin::m_Model{};

void ResultCoin::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\coin.obj");

}

void ResultCoin::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void ResultCoin::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");


	m_Velocity.y = 0.2f;
	m_Rotation.y = frand() * 30;
	m_Velocity.x = GetForward().x * (frand() * 0.03f);
	m_Velocity.z = GetForward().z * (frand() * 0.03f);
}

void ResultCoin::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ResultCoin::Update()
{
	GameObject::Update();
	
	//重力
	float groundHeight = 0.0f;	//地面の高さ
	m_Velocity.y -= 0.005f;		//落ちる

	//接地
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f) {
		m_Position.y = groundHeight;
		m_Velocity *= 0.0f;
	}

	//自動回転
	m_Rotation.y += 0.03f;

	//移動
	m_Position += m_Velocity;
}

void ResultCoin::Draw()
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

	m_Model->Draw();
}
