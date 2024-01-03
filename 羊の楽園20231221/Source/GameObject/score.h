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

	int  m_Count      = 0;
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
	void DrawData(float vx, float vy, float width, float height ,int num);

	void AddCount(int Count) { m_Count += Count; }
	void SetCount(int Count) { m_Count = Count; }

	void AddCountCoin(int CountCoin) { m_CountCoin += CountCoin; }
	void SetCountCoin(int CountCoin) { m_CountCoin = CountCoin; }
	void AddCountDay(int CountDay) { m_CountDay += CountDay; }
	void SetCountDay(int CountDay) { m_CountDay = CountDay; }
	void SetLifeF(int Life,int FullLife) { m_Life = Life; m_FullLife = FullLife;}
	void SetCountCombo(int Count)  { m_CountCombo = Count; }
	void SetTimeZone(int TimeZone) { m_TimeZone = TimeZone; }

	int  GetCount() { return m_Count; }
	int  GetFullCount() { return m_FullCount; }
	int  GetCountCoin() { return m_CountCoin; }
	int  GetCountDay() { return m_CountDay; }

};