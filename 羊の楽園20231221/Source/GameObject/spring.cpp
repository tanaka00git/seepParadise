#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\spring.h"
#include "..\App\manager.h"
#include "..\Scene\scene.h"
#include "..\GameObject\camera.h"
#include "..\App\model.h"
#include "..\GameObject\player.h"
#include "..\GameObject\follow.h"

Model* Spring::m_Model{};

void Spring::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\spring.obj");
}

void Spring::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Spring::Init()
{
	m_Scale.y = 0.1f;
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");
}

void Spring::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Spring::Update()
{
	GameObject::Update();

	m_DaathTime--;
	if(m_DaathTime > 0)
	{	
		//ぬるぬる出現
		SmoothAppearance(true);
	}
	else
	{
		SmoothAppearance(false);
		if (m_Scale.x <= 0.0f) { SetDestroy(); }
	}

	//データ取得
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();
	auto follows = scene->GetGameObjects<Follow>();

	//プレイヤーの距離を取得
	D3DXVECTOR3 direction = m_Position - player->GetPosition();
	D3DXVECTOR3 scale = player->GetScale();
	float length = D3DXVec3Length(&direction);
	if (length < scale.x)
	{
		player->SetJump();
	}

	for (Follow* follow : follows) {
		if (follow->GetState() != FOLLOW_STATE::FREE) {
			D3DXVECTOR3 position = follow->GetPosition();
			D3DXVECTOR3 scale = follow->GetScale();
			D3DXVECTOR3 direction = m_Position - position;
			float length = D3DXVec3Length(&direction);

			if (length < scale.x)
			{
				follow->SetJump();
			}
		}
	}
}

void Spring::Draw()
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

void Spring::SmoothAppearance(bool growing)
{
	if (growing)
	{
		m_Scale.x += m_OriginalScale.x / 20;
		m_Scale.y += m_OriginalScale.y / 20;
		m_Scale.z += m_OriginalScale.y / 20;
		if (m_Scale.x >= m_OriginalScale.x) { m_Scale.x = m_OriginalScale.x; }
		if (m_Scale.y >= m_OriginalScale.y) { m_Scale.y = m_OriginalScale.y; }
		if (m_Scale.z >= m_OriginalScale.z) { m_Scale.z = m_OriginalScale.z; }
	}
	else
	{
		m_Scale.x -= m_OriginalScale.y / 20;
		m_Scale.y -= m_OriginalScale.y / 20;
		m_Scale.z -= m_OriginalScale.y / 20;
		if (m_Scale.x <= 0.0f) { m_Scale.x = 0.0f; }
		if (m_Scale.y <= 0.0f) { m_Scale.y = 0.0f; }
		if (m_Scale.z <= 0.0f) { m_Scale.z = 0.0f; }
	}
}
