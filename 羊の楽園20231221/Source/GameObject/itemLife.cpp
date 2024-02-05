#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\itemLife.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\player.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\infoLog.h"
#include "..\GameObject\shine2.h"
#include "..\App\model.h"

Model*ItemLife::m_Model{};
Audio*ItemLife::m_SE_PowerUp{};
Audio*ItemLife::m_SE_Get{};

void ItemLife::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\item2.obj");

	m_SE_PowerUp = new Audio();
	m_SE_PowerUp->Load("asset\\audio\\ステータス上昇.wav");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\ぱくっ.wav");
}

void ItemLife::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void ItemLife::Draw()
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

void ItemLife::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(11, 3, D3DXVECTOR3(900, 480, 0));			//インフォ発生
	DamageFade* damageFade = scene->AddGameObject<DamageFade>(2);
	damageFade->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	player->AddFullLife(+1);
	player->AddLife(+1);

	m_SE_PowerUp->Play(1.0f);
	m_SE_Get->Play(1.0f);

	scene->AddGameObject<Shine2>(1)->SetPosition(m_Position);//エフェクト
}
