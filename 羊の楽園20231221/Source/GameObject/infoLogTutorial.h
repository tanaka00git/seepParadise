#pragma once
#include "..\GameObject\gameObject.h"

class InfoLogTutorial : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Sprite* m_Sprite_Tutorial1{};
	class Sprite* m_Sprite_Tutorial2{};
	class Sprite* m_Sprite_Tutorial3{};
	class Sprite* m_Sprite_Check{};

	int   m_Log    = 1;
	float m_Alpha  = 1.0f;
	float m_Alpha2 = 0.0f;
	float m_Alpha3 = 0.0f;
	float m_Alpha4 = 0.0f;

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0, 0, 0);

	int	 m_ControlTime = 0;
	bool m_Log1        = false;
	bool m_LogCharge   = false;
	bool m_LogAttack   = false;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	int GetTutorialLog() { return m_Log; };
	void Log1() { m_Log1 = true; };
	void LogChargeTrue() { m_LogCharge = true; };
	void LogAttackTrue() { m_LogAttack = true; };

};