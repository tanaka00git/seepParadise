#pragma once
#include "..\App\component.h"
#include <list>

class GameObject
{
protected:
	bool	m_Destroy = false;//消す予約

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	std::list<Component*> m_Component;//STLのリスト構造

public:
	void SetDestroy() { m_Destroy = true; }
	bool Destroy();

	void SetPosition(D3DXVECTOR3 Position){m_Position = Position;}
	void SetRotation(D3DXVECTOR3 Rotation){m_Rotation = Rotation;}
	void SetScale(D3DXVECTOR3 Scale){m_Scale = Scale;}

	D3DXVECTOR3 GetPosition(){return m_Position;}
	D3DXVECTOR3 GetScale(){return m_Scale;}
	D3DXVECTOR3 GetRotation(){return m_Rotation;}
	D3DXVECTOR3 GetForward();
	D3DXVECTOR3 GetRight();
	D3DXVECTOR3 GetUp();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	template<typename T>//テンプレート関数
	T* AddComponent();

};

template<typename T>
inline T* GameObject::AddComponent()
{
	Component* component = new T();
	m_Component.push_back(component);
	component->Init();

	return (T*)component;
}
