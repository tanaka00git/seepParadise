#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\App\compModel.h"

void CompModel::Init(const char* ModelName)
{
	m_Model = new Model();
	m_Model->Load(ModelName);
}

void CompModel::Uninit()
{
	m_Model->Unload();
	delete m_Model;
}

void CompModel::Update()
{
}

void CompModel::Draw()
{
	m_Model->Draw();

}
