#pragma once
#include "..\GameObject\breakObject.h"

class Fence : public BreakObject
{
private:
	static class Model* m_Model;

	//ä÷êî
	void UpdateDeath() override;

public:
	static void Load();
	static void Unload();
	
	void Init() override;
	void Draw() override;
};