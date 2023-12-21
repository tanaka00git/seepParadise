#pragma once
#include "..\App\component.h"
#include "..\App\model.h"

class CompModel : public Component
{
protected:
	Model* m_Model{};

public:
	void Init(const char* ModelName);
	void Uninit();
	void Update();
	void Draw();

};