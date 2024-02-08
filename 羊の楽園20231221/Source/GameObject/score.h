#pragma once
#include "..\GameObject\gameObject.h"

class Score : public GameObject
{
private:

	ID3D11Buffer*m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int  m_CountSeep  = 0;
	int  m_FullCount  = 0;
	int  m_CountCoin  = 0;
	int  m_CountDay   = 0;
	int  m_CountCombo = 0;
	bool m_TimeZone   = false;
	int  m_Life       = 0;
	int  m_FullLife   = 0;


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void DrawCountSeep();
	void DrawCountCoin();
	void DrawCountCombo();
	void DrawTimeZone();
	void DrawLife();
	void DrawData(float vx, float vy, float width, float height ,int num);

	void AddCount(int countSeep) { m_CountSeep += countSeep; }
	void SetCount(int countaSeep) { m_CountSeep = countaSeep; }

	void AddCountCoin(int countCoin) { m_CountCoin += countCoin; }
	void SetCountCoin(int countCoin) { m_CountCoin = countCoin; }
	void AddCountDay(int countDay) { m_CountDay += countDay; }
	void SetCountDay(int countDay) { m_CountDay = countDay; }
	void SetLifeF(int life,int fullLife) { m_Life = life; m_FullLife = fullLife;}
	void SetCountCombo(int count)  { m_CountCombo = count; }
	void SetTimeZone(bool timeZone) { m_TimeZone = timeZone; }

	int  GetCount() { return m_CountSeep; }
	int  GetFullCount() { return m_FullCount; }
	int  GetCountCoin() { return m_CountCoin; }
	int  GetCountDay() { return m_CountDay; }

};