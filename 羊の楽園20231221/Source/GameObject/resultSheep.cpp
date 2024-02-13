#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\input.h"
#include "..\GameObject\ResultSheep.h"
#include "..\GameObject\player.h"
#include "..\GameObject\box.h"
#include "..\GameObject\cylinder.h"
#include "..\GameObject\chest.h"
#include "..\GameObject\shadow.h"
#include "..\GameObject\camera.h"
#include "..\App\model.h"

Model*ResultSheep::m_Model{};
#define GRAVITY 0.005f

void ResultSheep::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\seep1.obj");
}

void ResultSheep::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
}

void ResultSheep::Init()
{
	m_Rotation.y = frand() * 2 * D3DX_PI;

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void ResultSheep::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}


void ResultSheep::Update()
{
	GameObject::Update();
	Scene* scene = Manager::GetScene();

	//データ取得
	auto resultSheeps = scene->GetGameObjects<ResultSheep>();

	m_OrientationTime += 0.1f;
	if (m_OrientationTime > m_NextRotTime) 
	{
		m_Rotation.y += (0.02f * m_NextRot);
	}
		
	if (m_OrientationTime > m_NextRotTime + 5.0f) {
		int a = irand(0,2);
		if (a == 1) {
			m_NextRot *= -1;
		}
		m_NextRotTime = frand()* 30.0f + 10.0f;
		m_OrientationTime = 0.0f;
	}
	m_Velocity.x = GetForward().x * 0.003f;
	m_Velocity.z = GetForward().z * 0.003f;

	//野良羊との関係性
	for (ResultSheep * resultSheep : resultSheeps)
	{
		D3DXVECTOR3 position = resultSheep->GetPosition();
		D3DXVECTOR3 scale = resultSheep->GetScale();
		D3DXVECTOR3 direction = m_Position - position;
		float flength = D3DXVec3Length(&direction);

		if (flength < scale.x) 
		{
			m_Velocity.x += (m_Position.x - resultSheep->m_Position.x) * 0.02f;	//他の羊との接触でズレる
			m_Velocity.z += (m_Position.z - resultSheep->m_Position.z) * 0.02f;
		}
	}
	//重力
	m_Velocity.y -= GRAVITY;

	//障害物との衝突判定
	float groundHeight = 0.0f;

	//移動
	m_Position += m_Velocity;	

	//接地
	if (m_Position.y < groundHeight && m_Velocity.y < 0.0f)
	{
		m_Position.y = groundHeight;
		m_Velocity.y = 0.0f;
	}

	//疑似アニメ
	if (m_AnimeTime > 1.5f) 
	{
		m_Rotation.x = (0.03f*m_AnimePause);
		m_AnimePause = !m_AnimePause;
		m_AnimeTime = 0.0f;
	}
	m_AnimeTime += 0.1f;
}


void ResultSheep::Draw()
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
