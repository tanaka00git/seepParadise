#pragma once
#include "scene.h"

enum class RESULT_STATE
{
	NORMAL,
	FADE,
};

class Result : public Scene
{
public:
	RESULT_STATE m_ResultState = RESULT_STATE::NORMAL;
	class Fade* m_Fade{};
	class Audio* m_BGM{};
	class Audio* m_SE{};

	static int m_Day;
	static int m_Coin;
	static int m_Count;

	//ä÷êî
	void UpdateNormal();
	void UpdateFade();

public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	void SetDay(int day) { m_Day = day; };
	void SetCoin(int coin) { m_Coin = coin; };
	void SetCount(int count) { m_Count = count; };

};