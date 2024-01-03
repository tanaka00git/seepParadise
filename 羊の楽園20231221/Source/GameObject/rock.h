#pragma once
#include "..\GameObject\breakObject.h"

class Rock : public BreakObject
{
private:
	static class Model* m_Model;

	//�֐�
	void UpdateDeath() override;

public:
	static void Load();
	static void Unload();
	void Draw() override;

};