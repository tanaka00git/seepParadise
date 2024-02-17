#pragma once
#include "..\GameObject\human.h"

class HumanSpring : public Human
{
private:
	static class Model* m_ModelHumanSpring;

	void Make(D3DXVECTOR3 objectPosition) override;
public:
	static void Load();
	static void Unload();

	void Draw()override;
};