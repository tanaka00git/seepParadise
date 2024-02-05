#pragma once
#include "..\GameObject\wolf.h"

class DisasterWolf : public Wolf
{
private:
	static class Model* m_ModelClown;
	static class Audio* m_SE_SuperAttack;
	class AttackMarker* m_AttackMarker{};

	bool  m_Disaster = false;									//ŠëŒ¯ŒÂ‘Ì”»•Ê—p
	int   m_DisasterCount = 0;									//“G‚ªoŒ»‚·‚é
	int	  m_SuparChargeCount = 0;

	//ŠÖ”
	void UpdateTargeting() override;
	void UpdateSuperCharge() override;
	void UpdateSuperAttack() override;
public:
	static void Load();
	static void Unload();

	void Init()override;
	void Update()override;
	void Draw()override;
	void SetEnemyData(int data) override;
};