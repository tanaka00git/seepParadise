#include "..\App\main.h"
#include "..\GameObject\gameObject.h"

//定数
const D3DXVECTOR3 INITIAL_POSITION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 INITIAL_ROTATION = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 INITIAL_SCALE = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

void GameObject::Init()
{
	m_Destroy = false;

	// 各変数の初期化
	m_Position = INITIAL_POSITION;
	m_Rotation = INITIAL_ROTATION;
	m_Scale = INITIAL_SCALE;

	// コンポーネントの初期化
	for (Component* component : m_Component)
	{
		component->Init();
	}
}

void GameObject::Uninit()
{
	for (Component* component : m_Component)//範囲forループ
	{
		component->Uninit();
		delete	component;
	}

	m_Component.clear();//リスト構造の削除
}

void GameObject::Update()
{
	for (Component* component : m_Component) { component->Update(); }
}

void GameObject::Draw()
{
	for (Component* component : m_Component) { component->Draw(); }
}

bool GameObject::Destroy()
{
	if (m_Destroy)
	{
		Uninit();
		delete this;
		return true;
	}
	else { return false; }
}

D3DXVECTOR3 GameObject::GetForward()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

	D3DXVECTOR3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;
	return forward;
}

D3DXVECTOR3 GameObject::GetRight()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

	D3DXVECTOR3 forward;
	forward.x = rot._11;
	forward.y = rot._12;
	forward.z = rot._13;
	return forward;
}

D3DXVECTOR3 GameObject::GetUp()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

	D3DXVECTOR3 up;
	up.x = rot._21;
	up.y = rot._22;
	up.z = rot._23;
	return up;
}
