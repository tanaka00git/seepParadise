#pragma once
#include "..\GameObject\breakObject.h"

class Tree : public BreakObject
{
private:
	static class Model* m_Model;
	static class Model* m_ModelAppleTree;
	class Shadow* m_Shadow{};

	bool m_ApplePossession = false;	//ƒŠƒ“ƒS‚ğŠ‚µ‚Ä‚¢‚é‚©

	//ŠÖ”
	void UpdateDead() override;

public:
	static void Load();
	static void Unload();
	void Init() override;
	void Update() override;
	void Draw() override;

};