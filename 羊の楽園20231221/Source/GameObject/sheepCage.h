#pragma once
#include "..\GameObject\breakObject.h"

class SheepCage : public BreakObject
{
private:
	static class Model* m_ModelCage;
	static class Model* m_ModelCageSheep;
	static class Audio* m_SE_Chest;

	float m_SheepRotationY = 0.0;

	//ä÷êî
	void UpdateDead() override;

public:
	static void Load();
	static void Unload();
	void Init() override;
	void Draw() override;

};