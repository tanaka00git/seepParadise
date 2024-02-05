#pragma once
#include "..\GameObject\wolf.h"

class DisasterWolf : public Wolf
{
private:
	static class Model* m_ModelClown;

	bool  m_Disaster = false;									//ŠëŒ¯ŒÂ‘Ì”»•Ê—p
	int   m_DisasterCount = 0;									//“G‚ªoŒ»‚·‚é

	//ŠÖ”
	void DisasterMove();
	void UpdateTargeting() override;
public:
	static void Load();
	static void Unload();

	virtual void Draw()override;
	void SetEnemyData(int data) override;
};