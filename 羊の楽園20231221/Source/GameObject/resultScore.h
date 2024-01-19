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

	int m_CountSeep{};
	int m_CountCoin{};
	int m_CountDay{};
	int m_CountCombo{};

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddCount(int countSeep) { m_CountSeep += countSeep; }
	void SetCount(int countSeep) { m_CountSeep = countSeep; }
	void AddCountCoin(int countCoin) { m_CountCoin += countCoin; }
	void SetCountCoin(int countCoin) { m_CountCoin = countCoin; }
	void AddCountDay(int countDay) { m_CountDay += countDay; }
	void SetCountDay(int countDay) { m_CountDay = countDay; }
	void SetCountCombo(int countCombo) { m_CountCombo = countCombo; }

};