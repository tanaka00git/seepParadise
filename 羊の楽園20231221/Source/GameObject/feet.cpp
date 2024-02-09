#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\feet.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\score.h"
#include "..\GameObject\Shadow.h"
#include "..\GameObject\player.h"
#include "..\GameObject\feetEffect.h"
#include "..\App\model.h"

Model*Feet::m_Model{};
Audio*Feet::m_SE_Get{};

#define MAX_CHARGE 1000
#define SUBTRACTION_CHARGE 30
#define MIN_CHARGE 180
#define SCALE 0.5f

void Feet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\feed.obj");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\パパッ.wav");

}

void Feet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Feet::Init()
{
	ItemObject::Init();

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void Feet::Draw()
{
	GameObject::Draw();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x * SCALE, m_Scale.y * SCALE, m_Scale.z * SCALE);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(&world);
	m_Model->Draw();
}

void Feet::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	Player* player = scene->GetGameObject<Player>();
	int charge = MAX_CHARGE - (score->GetCount() * SUBTRACTION_CHARGE);
	charge = (charge <= MIN_CHARGE) ? MIN_CHARGE : charge;
	player->AddCharge(charge);

	m_SE_Get->Play(1.0f);

	scene->AddGameObject<FeetEffect>(1)->SetPosition(m_Position);//コインエフェクト
}