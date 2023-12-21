#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\App\input.h"
#include "..\App\audio.h"
#include "..\Scene\scene.h"
#include "..\Scene\title.h"

Scene* Manager::m_Scene{};
Scene* Manager::m_NextScene{};

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	Audio::InitMaster();

	SetScene<Title>();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if(m_NextScene)
	{
		if (m_Scene)
		{
			
			m_Scene->Uninit();
			delete m_Scene;
		}
		m_Scene = m_NextScene;
		m_Scene->Init();

		m_NextScene = nullptr;
	}

	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();
}