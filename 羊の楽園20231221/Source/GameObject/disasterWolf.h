#pragma once
#include "..\GameObject\wolf.h"

class DisasterWolf : public Wolf
{
private:
	static class Model* m_ModelClown;

	bool  m_Disaster = false;									//�댯�̔��ʗp
	int   m_DisasterCount = 0;									//�G���o������

	//�֐�
	void DisasterMove();
	void UpdateTargeting() override;
public:
	static void Load();
	static void Unload();

	virtual void Draw()override;
	void SetEnemyData(int data) override;
};