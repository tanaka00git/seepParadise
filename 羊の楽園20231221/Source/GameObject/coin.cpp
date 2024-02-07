#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\coin.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\score.h"
#include "..\GameObject\player.h"
#include "..\GameObject\infoLog.h"
#include "..\GameObject\shine.h"
#include "..\App\model.h"

Model*Coin::m_Model{};
Audio*Coin::m_SE_Get{};

#define MAX_CHARGE 1000
#define SUBTRACTION_CHARGE 50
#define MIN_CHARGE 200

void Coin::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\coin.obj");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\硬貨・渡す02b.wav");

}

void Coin::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Coin::Draw()
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

void Coin::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Score* score = scene->GetGameObject<Score>();

	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(3, 2, D3DXVECTOR3(50.0f, 50.0f, 0.0f));
	score->AddCountCoin(+1);
	
	Player* player = scene->GetGameObject<Player>();
	int charge = MAX_CHARGE - (score->GetCount() * SUBTRACTION_CHARGE);
	charge = (charge <= MIN_CHARGE) ? MIN_CHARGE : charge;
	player->AddCharge(charge);

	m_SE_Get->Play(1.0f);

	scene->AddGameObject<Shine>(1)->SetPosition(m_Position);//コインエフェクト
}