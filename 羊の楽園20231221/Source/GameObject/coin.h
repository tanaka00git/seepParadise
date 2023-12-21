#pragma once
#include "..\GameObject\itemObject.h"

class Coin : public ItemObject
{
private:
	static class Model* m_Model;
	static class Audio* m_SE_Get;
public:
	static void Load();
	static void Unload();
	void Draw()override;
	void MoveGet()override;
};