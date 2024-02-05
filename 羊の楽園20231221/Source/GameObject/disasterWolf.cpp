#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\wolf.h"
#include "..\GameObject\disasterWolf.h"
#include "..\App\model.h"
#include "..\GameObject\camera.h"

Model* DisasterWolf::m_ModelClown{};

void DisasterWolf::Load()
{
	m_ModelClown = new Model();
	m_ModelClown->Load("asset\\model\\wolfClown1.obj");
}

void DisasterWolf::Unload()
{
	m_ModelClown->Unload();
	delete m_ModelClown;
	m_ModelClown = nullptr;
}

void DisasterWolf::Init()
{
	Wolf::Init();

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\pixelLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\pixelLightingPS.cso");
}

void DisasterWolf::Draw()
{
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>();
	if (!camera->CheckView(m_Position, m_Scale)) { return; }

	GameObject::Draw();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x + m_AnimeRotationX, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->DrawColor(m_Color, m_TextureEnable);
	m_ModelClown->DrawColor(m_Color, m_TextureEnable);
}

void DisasterWolf::DisasterMove()
{
	//�������ł��Ȃ��悤�ɓK���Ȓl�ŏ㏑��
	m_DaathTime = 1000;

	//��莞�Ԍo�ƃI�[�g�œG�𐶂ݏo��
	Scene* scene = Manager::GetScene();
	m_DisasterCount++;
	if (m_DisasterCount >= 150)
	{
		Wolf* wolf = scene->AddGameObject<Wolf>(1);
		wolf->SetEnemyData(1);
		wolf->SetPosition(m_Position);
		m_DisasterCount = 0;
	}
}

void DisasterWolf::UpdateTargeting()
{
	Wolf::UpdateTargeting();

	//�ЊQ�T��p�C�x���g
	DisasterMove();
}


void DisasterWolf::SetEnemyData(int data)
{
	m_Item = false;
	m_BiteCount = 2;
	m_FullLife = 80;
	m_Speed = 0.04f;
	m_CoinDrop = 30;
	m_StanGuard = 40;
	m_OriginalScale = D3DXVECTOR3(2.9f, 2.9f, 2.9f);	//�L�����̃T�C�Y
	m_BarScale = D3DXVECTOR3(5.0f, 1.0f, 1.0f);			//HP�o�[�̃T�C�Y
	m_Tracking = 200.0f;								//�ǔ��͈�
	m_Life = m_FullLife;
}
