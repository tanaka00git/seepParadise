#pragma once
#include "..\GameObject\itemObject.h"

class Feet : public ItemObject
{
private:
	static class Model* m_Model;
	static class Audio* m_SE_Get;

	//ä÷êî
	void MoveGet()override;

public:
	static void Load();
	static void Unload();
	void Init()override;
	void Draw()override;
};