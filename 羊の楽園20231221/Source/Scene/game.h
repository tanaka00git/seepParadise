#pragma once
#include "scene.h"

class DisasterWolf;

enum class GAME_STATE
{
	START,
	NORMAL,
	CLEAR,
	FADE,
};

class Game : public Scene
{
private:
	std::vector<DisasterWolf*> wolves;  // ウルフのリスト

	GAME_STATE m_GameState = GAME_STATE::START; // ゲームステート
	class Audio* m_BGM{};
	class Audio* m_BGM_Night{};
	class Audio* m_SE_Whistle{};
	class Audio* m_SE_SheepCry{};
	class Audio* m_SE_Bell{};
	class Audio* m_SE_Gong{};
	class Audio* m_SE_WolfCry{};
	class Fade* m_Fade{};
	class TimeFade* m_TimeFade{};
	class DamageFade* m_DamageFade{};

	int m_GameTime			= 0;
	int m_GameTimeSeconds	= 0;
	int m_Day				= 0;

	int  m_StartTextTime	= 0;
	int  m_NawEvent			= 0;
	bool m_Event1			= false;
	bool m_Event2			= false;
	int	 m_ClearTime		= 0;

	//関数
	void UpdateStart();
	void UpdateNormal();
	void UpdateClear();
	void UpdateFade();
	void TimeSecondsUpdate();
	void TimeEvent_Time2Loop();
	void TimeEvent_Time30();
	void TimeEvent_Time15or45();
	void TimeEvent_Time60();
	void DisasterWolfEmitter();

public:
	void Init() override;
	void Uninit() override;
	void Update() override;

};