#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\apple.h"
#include "..\GameObject\itemObject.h"
#include "..\App\audio.h"
#include "..\GameObject\score.h"
#include "..\GameObject\player.h"
#include "..\GameObject\damageFade.h"
#include "..\GameObject\infoLog.h"
#include "..\App\model.h"

Model*Apple::m_Model{};
Audio*Apple::m_SE_Get{};
Audio*Apple::m_SE_Heel{};

void Apple::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\apl.obj");
	m_SE_Get = new Audio();
	m_SE_Get->Load("asset\\audio\\ぱくっ.wav");
	m_SE_Heel = new Audio();
	m_SE_Heel->Load("asset\\audio\\シャランッ.wav");
}

void Apple::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Apple::Draw()
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

void Apple::MoveGet()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	InfoLog* infoLog = scene->AddGameObject<InfoLog>(2);
	infoLog->SetNum(5, 2, D3DXVECTOR3(100, -10, 0));
	DamageFade* damageFade = scene->AddGameObject<DamageFade>(2);
	damageFade->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.3f));
	player->AddLife(+1);

	m_SE_Get->Play(1.0f);
	m_SE_Heel->Play(1.0f);
}
