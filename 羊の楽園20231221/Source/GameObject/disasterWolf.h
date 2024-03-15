#pragma once
#include "..\GameObject\wolf.h"

class DisasterWolf : public Wolf
{
private:
	static class Model* m_ModelClown;
	static class Audio* m_SE_SuperAttack;
	class AttackMarker* m_AttackMarker{};

	int	  m_SuperChargeCount = 0;

	//�֐�
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