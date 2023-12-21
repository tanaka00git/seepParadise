#pragma once
#include "..\GameObject\gameObject.h"

class ResultScore : public GameObject
{
private:

	ID3D11Buffer*m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture{};

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	int m_Count{};
	int m_CountCoin{};
	int m_CountDay{};
	int m_CountCombo{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddCount(int Count) { m_Count += Count; }
	void SetCount(int Count) { m_Count = Count; }
	void AddCountCoin(int CountCoin) { m_CountCoin += CountCoin; }
	void SetCountCoin(int CountCoin) { m_CountCoin = CountCoin; }
	void AddCountDay(int CountDay) { m_CountDay += CountDay; }
	void SetCountDay(int CountDay) { m_CountDay = CountDay; }
	void SetCountCombo(int Count) { m_CountCombo = Count; }

};