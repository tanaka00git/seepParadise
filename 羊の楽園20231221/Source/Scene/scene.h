#pragma once
#include "..\GameObject\gameObject.h"
#include <list>
#include <vector>
#include <typeinfo>

class Scene
{
protected:
	std::list<GameObject*> m_GameObject[4];//レイヤー有のSTLのリスト構造

public:
	virtual void Init(){}
	virtual void Uninit()
	{
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])//範囲forループ
			{
				gameObject->Uninit();
				delete	gameObject;
				gameObject = nullptr;

			}

			m_GameObject[i].clear();//リスト構造の削除
		}
	}
	virtual void Update()
	{
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])//範囲forループ
			{
				gameObject->Update();
			}

			m_GameObject[i].remove_if([](GameObject*object) {return object->Destroy(); });//ラムダ式
		}
	}
	virtual void Draw()
	{
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* gameObject : m_GameObject[i])//範囲forループ
			{
				gameObject->Draw();

			}
		}
	}

	template<typename T>//テンプレート関数
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		m_GameObject[Layer].push_back(gameObject);
		gameObject->Init();

		return gameObject;
	}

	//一つのオブジェクト取得用
	template<typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				if (dynamic_cast<T*>(object) != nullptr)
				{
					return dynamic_cast<T*>(object);
				}
			}
		}
		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;
		for (int i = 0; i < 4; i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				T* castedObject = dynamic_cast<T*>(object);
				if (castedObject != nullptr)
				{
					objects.push_back(castedObject);
				}
			}
		}
		return objects;
	}

};