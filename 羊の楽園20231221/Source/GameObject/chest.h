#pragma once
#include "..\GameObject\breakObject.h"

class Chest : public BreakObject
{
private:
	static class Model* m_Model;
	static class Audio* m_SE_Chest;
	
	//ä÷êî
	void UpdateDead() override;

public:
	static void Load();
	static void Unload();
	void Draw() override;

};