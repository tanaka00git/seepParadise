#pragma once
#include "scene.h"

enum class TITLE_STATE
{
	PRESS_START,
	TARGET_GAME,
	TARGET_SHOP,
	TARGET_RULE,
	FADE,
};

class Title : public Scene
{
private:
	TITLE_STATE m_TitleState = TITLE_STATE::PRESS_START;
	class Audio* m_BGM{};
	class Audio* m_PointSE{};
	class Audio* m_LegiSE{};
	class Audio* m_PointMoveSE{};
	class Audio* m_MissSE{};
	class Audio* m_SE{};
	class Fade* m_Fade{};
	class TimeFade* m_TimeFade{};
	class TitleLogo* m_TitleLogo{};
	class TitleScore* m_TitleScore{};

	static bool m_Begin;		//初回起動用
	static bool m_DebugMode;	//デバッグモード判定
	static int  m_Coin;			//コインの引継ぎ
	static int  m_PlColor;		//羊カラー
	static int  m_PlClown;		//羊装備

	int m_SeepDropTime = 0;

	//関数
	void UpdatePressStart();
	void UpdateTargetGame();
	void UpdateTargetShop();
	void UpdateTargetRule();
	void UpdateFade();
	void TargetMove();

public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	void AddCoin(int coin) { m_Coin += coin; };

};