#pragma once
#include "..\GameObject\gameObject.h"

class Goal : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Model* m_Model;
	static class Audio* m_SE_Goal;
	class GoalScope* m_GoalScope{};

	D3DXVECTOR3 m_Velocity{};
	D3DXVECTOR3 m_ScopeScale{};

	//ä÷êî
	bool Collision();

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();
};