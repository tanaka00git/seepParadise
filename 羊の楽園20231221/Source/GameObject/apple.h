#pragma once
#include "..\GameObject\itemObject.h"

class Apple : public ItemObject
{
private:
	static class Model* m_Model;
	static class Audio* m_SE_Get;
	static class Audio* m_SE_Heel;
public:
	static void Load();
	static void Unload();
	void Draw() override;
	void MoveGet() override;
};