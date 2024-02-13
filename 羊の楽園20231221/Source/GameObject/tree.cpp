#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\tree.h"
#include "..\GameObject\breakObject.h"
#include "..\GameObject\shadow.h"
#include "..\App\audio.h"
#include "..\GameObject\feet.h"
#include "..\GameObject\Apple.h"
#include "..\GameObject\hpBarS.h"
#include "..\App\model.h"

Model* Tree::m_Model{};
Model* Tree::m_ModelAppleTree{};

#define DROP_RAIT 4
#define FEET_DROP 2
#define INITIAL_LIFE 3
#define INITIAL_HP_BAR_POS_Y 3.0f

void Tree::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\tree_a.obj");
	m_ModelAppleTree = new Model();
	m_ModelAppleTree->Load("asset\\model\\tree_a2.obj");
}

void Tree::Unload()
{
	m_Model->Unload();
	delete m_Model;
	m_Model = nullptr;
	m_ModelAppleTree->Unload();
	delete m_ModelAppleTree;
	m_Model = nullptr;
}

void Tree::Init()
{
	BreakObject::Init();

	//初期化
	m_Life = INITIAL_LIFE;				//体力
	m_FullLife = INITIAL_LIFE;			//最大体力
	m_HpBarPosY = INITIAL_HP_BAR_POS_Y;	//HPポジションy

	//リンゴを持ってる確率
	int randam;
	randam = irand(1, DROP_RAIT);
	if (randam == 1) { m_ApplePossession = true; }
	else{ m_ApplePossession = false; }

	m_Shadow = AddComponent<Shadow>();
}

void Tree::Update()
{
	BreakObject::Update();

	//影更新
	float groundHeight = 0.0f;
	D3DXVECTOR3 shadowPosition = m_Position;
	shadowPosition.y = groundHeight + 0.01f;
	m_Shadow->SetPosition(shadowPosition);
	m_Shadow->SetScale(D3DXVECTOR3(m_Scale.x, m_Scale.y, m_Scale.z));
}

void Tree::Draw()
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

	if(m_ApplePossession){m_ModelAppleTree->DrawColor(m_Color, m_TextureEnable);
	}
	else{m_Model->DrawColor(m_Color, m_TextureEnable);}
}

void Tree::UpdateDead()
{
	BreakObject::UpdateDead();

	//Init処理
	if (!m_DeleteInit)
	{
		Scene* scene = Manager::GetScene();

		m_DeleteInit = true;
		m_HpBarS->SetDraw(false);

		//リンゴドロップ
		if (m_ApplePossession)
		{
			Apple* apple = scene->AddGameObject<Apple>(1);
			apple->SetPosition(m_Position);
			apple->SetDrop();
		}

		//コインドロップ
		for (int i = 0; i <= FEET_DROP; i++)
		{
			Feet* feet = scene->AddGameObject<Feet>(1);
			feet->SetPosition(m_Position);
			feet->SetDrop();
		}
	}
}

