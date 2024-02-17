#include "..\App\main.h"
#include "..\GameObject\human.h"
#include "..\GameObject\humanSpring.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\App\model.h"
#include "..\GameObject\spring.h"
#include "..\GameObject\camera.h"

Model* HumanSpring::m_ModelHumanSpring{};

void HumanSpring::Load()
{
	m_ModelHumanSpring = new Model();
	m_ModelHumanSpring->Load("asset\\model\\humanSpring.obj");
}

void HumanSpring::Unload()
{
	m_ModelHumanSpring->Unload();
	delete m_ModelHumanSpring;
	m_ModelHumanSpring = nullptr;
}

void HumanSpring::Draw()
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

	m_ModelHumanSpring->DrawColor(m_Color, m_TextureEnable);
}

void HumanSpring::Make(D3DXVECTOR3 objectPosition)
{
	//バネ発生
	Scene* scene = Manager::GetScene();

	Spring* spring = scene->AddGameObject<Spring>(1);
	spring->SetPosition(objectPosition);
	spring->SetRotation(m_Rotation);
}
