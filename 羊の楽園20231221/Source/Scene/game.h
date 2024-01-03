#pragma once
#include "scene.h"

class Game : public Scene
{
private:
	class Audio* m_BGM{};
	class Audio* m_BGM_Night{};
	class Audio* m_SE_SeepCry{};
	class Audio* m_SE_Bell{};
	class Audio* m_SE_Gong{};
	class Audio* m_SE_WolfCry{};
	class Fade* m_Fade{};
	class TimeFade* m_TimeFade{};
	class DamageFade* m_DamageFade{};

	int m_GameTime		  = 0;
	int m_GameTimeSeconds = 0;
	int m_Day			  = 0;

	int m_NawEvent		  = 0;
	bool m_Event1		  = false;
	bool m_Event2		  = false;

	//ä÷êî
	void TimeSecondsUpdate();
	void TimeEvent_Time2Loop();
	void TimeEvent_Time20();
	void TimeEvent_Time10or30();
	void TimeEvent_Time40();
	void TimeEvent_Day4Loop();

public:
	void Init() override;
	void Uninit() override;
	void Update() override;

};