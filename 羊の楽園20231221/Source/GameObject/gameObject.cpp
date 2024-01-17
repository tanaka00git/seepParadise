#include "..\App\main.h"
#include "..\GameObject\gameObject.h"

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
