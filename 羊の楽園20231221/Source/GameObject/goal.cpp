#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\App\audio.h"
#include "..\Scene\scene.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\goal.h"
#include "..\GameObject\goalScope.h"
#include "..\GameObject\player.h"
#include "..\GameObject\follow.h"
#include "..\App\model.h"
#include <algorithm>
using std::max;

Model* Goal::m_Model{};
Audio* Goal::m_SE_Goal{};

#define SCOPE_SIZE 10.0f
#define ROTATION_SPEED 0.1f
#define STAGE_MAKE_XY 110.0f
#define FOLLOW_MAKE_POSITION_Y 1.1f
#define MAKE_MINMUM_DISTANCE 15.0f	//生成されるプレイヤーとゴールの距離の最低距離

void Goal::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\target.obj");
	m_SE_Goal = new Audio();
	m_SE_Goal->Load("asset\\audio\\鈴を鳴らす.wav");
}

void Goal::Unload()
{
	m_Model->Unload();
	delete m_Model;

}

void Goal::Init()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\vertexLightingPS.cso");

	m_ScopeScale = D3DXVECTOR3(SCOPE_SIZE, SCOPE_SIZE, SCOPE_SIZE);

	m_GoalScope = AddComponent<GoalScope>();
	m_GoalScope->SetScale(m_ScopeScale);

	SetNextGoal();
}

void Goal::Uninit()
{
	GameObject::Uninit();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Goal::Update()
{
	GameObject::Update();

	m_Rotation.y += ROTATION_SPEED;

	//ゴールスコープ移動
	if (m_GoalScope)
	{
		D3DXVECTOR3 scopePosition = m_Position;
		scopePosition.y += 0.1f;
		m_GoalScope->SetPosition(scopePosition);
		m_GoalScope->SetScale(m_ScopeScale);
	}

	if (Collision())
	{
		Scene* scene = Manager::GetScene();
		m_SE_Goal->Play(1.0f);

		//プレイヤー座標
		Player* player = scene->GetGameObject<Player>();
		D3DXVECTOR3 PLPos = player->GetPosition();

		//仲間を増やす
		for (int i = 1; i <= 10; i++)
		{
			Follow* follow = scene->AddGameObject<Follow>(1);
			follow->SetPosition(D3DXVECTOR3(PLPos.x, PLPos.y + FOLLOW_MAKE_POSITION_Y, PLPos.z));
			follow->SetDrop();
		}

		//画面を揺らす
		Camera* camera = scene->GetGameObject<Camera>();
		camera->SetShake(0.0f, 0.4f);

		//次の位置にゴールを設定する
		SetNextGoal();
	}
}

void Goal::Draw()
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

bool Goal::Collision()
{
	Scene* scene = Manager::GetScene();

	//接触されてるプレイヤーとフォロー取得用
	Player* player = scene->GetGameObject<Player>();
	auto follows = scene->GetGameObjects<Follow>();

	//プレイヤー用
	if (player)
	{
		if (player->GetCharacterState() == CHARACTER_STATE::ALIVE)
		{
			D3DXVECTOR3 position = player->GetPosition();
			D3DXVECTOR3 scale = player->GetScale();
			if (m_Position.x - m_ScopeScale.x * 0.5f < position.x - scale.x * 0.5f &&
				position.x + scale.x * 0.5f < m_Position.x + m_ScopeScale.x * 0.5f &&
				m_Position.z - m_ScopeScale.z * 0.5f < position.z - scale.z * 0.5f &&
				position.z + scale.z * 0.5f < m_Position.z + m_ScopeScale.z * 0.5f)
			{
				return true;
			}
		}
	}

	//味方用
	for (Follow* follow : follows)
	{
		if (follow->GetState() != FOLLOW_STATE::FREE)
		{
			D3DXVECTOR3 position = follow->GetPosition();
			D3DXVECTOR3 scale = follow->GetScale();
			if (m_Position.x - m_ScopeScale.x * 0.5f < position.x - scale.x * 0.5f &&
				position.x + scale.x * 0.5f < m_Position.x + m_ScopeScale.x * 0.5f &&
				m_Position.z - m_ScopeScale.z * 0.5f < position.z - scale.z * 0.5f &&
				position.z + scale.z * 0.5f < m_Position.z + m_ScopeScale.z * 0.5f)
			{
				return true;
			}
		}
	}

	return false;
}

void Goal::SetNextGoal()
{
	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	D3DXVECTOR3 newGoalPosition;
	D3DXVECTOR3 position = player->GetPosition();
	D3DXVECTOR3 scale = player->GetScale();
	D3DXVECTOR3 direction;
	float length;

	do {
		newGoalPosition = D3DXVECTOR3(frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2, 0.0f, frand() * STAGE_MAKE_XY - STAGE_MAKE_XY / 2);
		direction = newGoalPosition - position;  // プレイヤーから新しいゴールへのベクトルを計算
		length = D3DXVec3Length(&direction);
	} while (length < MAKE_MINMUM_DISTANCE);  // 最低距離離れるまで再試行


	SetPosition(newGoalPosition);
}
