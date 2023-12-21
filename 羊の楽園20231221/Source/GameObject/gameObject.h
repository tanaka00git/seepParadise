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
	bool Destroy(){
		if(m_Destroy)
		{
		Uninit();
		delete this;
		return true;
		}
		else{return false;}
	}

	void SetPosition(D3DXVECTOR3 Position){m_Position = Position;}
	void SetRotation(D3DXVECTOR3 Rotation){m_Rotation = Rotation;}
	void SetScale(D3DXVECTOR3 Scale){m_Scale = Scale;}

	D3DXVECTOR3 GetPosition(){return m_Position;}
	D3DXVECTOR3 GetScale(){return m_Scale;}
	D3DXVECTOR3 GetRotation(){return m_Rotation;}
	D3DXVECTOR3 GetForward()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._31;
		forward.y = rot._32;
		forward.z = rot._33;
		return forward;
	}
	D3DXVECTOR3 GetRight()
	{
		D3DXMATRIX rot;
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);

		D3DXVECTOR3 forward;
		forward.x = rot._11;
		forward.y = rot._12;
		forward.z = rot._13;
		return forward;
	}

	virtual void Init(){};
	virtual void Uninit() 
	{
		for (Component* component : m_Component)//範囲forループ
		{
			component->Uninit();
			delete	component;
		}

		m_Component.clear();//リスト構造の削除
	}
	virtual void Update()
	{
		for (Component* component : m_Component){component->Update();}
	}
	virtual void Draw()
	{
		for (Component* component : m_Component){component->Draw();}
	};

	template<typename T>//テンプレート関数
	T* AddComponent()
	{
		Component* component = new T();
		m_Component.push_back(component);
		component->Init();

		return (T*)component;
	}

};