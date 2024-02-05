#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\wolf.h"
#include "..\GameObject\disasterWolf.h"
#include "..\App\model.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\timeFade.h"
#include "..\GameObject\AttackMarker.h"
#include "..\App\audio.h"

Model* DisasterWolf::m_ModelClown{};
Audio* DisasterWolf::m_SE_SuperAttack{};

void DisasterWolf::Load()
{
	m_ModelClown = new Model();
	m_ModelClown->Load("asset\\model\\wolfClown1.obj");
	m_SE_SuperAttack = new Audio();
	m_SE_SuperAttack->Load("asset\\audio\\パンチの衣擦れ1.wav");
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

	m_AttackMarker = AddComponent<AttackMarker>();
}

void DisasterWolf::Update()
{
	Wolf::Update();
	
	//アタックマーカーの移動
	D3DXVECTOR3 attackMarkerPosition = m_Position;
	m_AttackMarker->SetPosition(attackMarkerPosition);
	m_AttackMarker->SetRotation(m_Rotation);

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

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x + m_AnimeRotationX, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);

	m_Model->DrawColor(m_Color, m_TextureEnable);
	m_ModelClown->DrawColor(m_Color, m_TextureEnable);
}

void DisasterWolf::UpdateTargeting()
{
	Wolf::UpdateTargeting();

	//自動消滅しないように適当な値で上書き
	m_DaathTime = 1000;

	//一定時間経つとオートで敵を生み出す
	Scene* scene = Manager::GetScene();
	m_DisasterCount++;
	if (m_DisasterCount >= 150)
	{
		for (int i = 0; i < 10; i++)
		{
			Wolf* wolf = scene->AddGameObject<Wolf>(1);
			wolf->SetEnemyData(1);
			wolf->SetPosition(m_Position);
			wolf->SetDrop();
			m_DisasterCount = 0;
		}
	}

	m_SuparChargeCount++;
	if (m_SuparChargeCount >= 60)
	{
		m_SuparChargeCount = 0;
		m_WolfState = WOLF_STATE::SUPER_CHARGE;
	}
}

void DisasterWolf::UpdateSuperCharge()
{
	m_Velocity *= 0;
	m_AttackMarker->SetScale(D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z * 5));

	m_SuparChargeCount++;
	if (m_SuparChargeCount >= 60)
	{
		m_SE_SuperAttack->Play(1.0f ,false);
		m_WolfState = WOLF_STATE::SUPER_ATTACK;
	}
}

void DisasterWolf::UpdateSuperAttack()
{
	m_Velocity.x = GetForward().x * (m_Speed) * 6;
	m_Velocity.z = GetForward().z * (m_Speed) * 6;

	m_AttackMarker->SetScale(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	m_SuparChargeCount--;
	if (m_SuparChargeCount <= 0)
	{
		m_SuparChargeCount = 0;
		m_WolfState = WOLF_STATE::TARGETING;
	}
}

void DisasterWolf::SetEnemyData(int data)
{
	m_Item = false;
	m_BiteCount = 2;
	m_FullLife = 80 * data;
	m_Speed = 0.04f;
	m_CoinDrop = 30 * data;
	m_StanGuard = 40 * data;
	m_OriginalScale = D3DXVECTOR3(6.0f, 6.0f, 6.0f);	//キャラのサイズ
	m_BarScale = D3DXVECTOR3(5.0f, 1.0f, 1.0f);			//HPバーのサイズ
	m_Tracking = 200.0f;								//追尾範囲
	m_Life = m_FullLife;
}

