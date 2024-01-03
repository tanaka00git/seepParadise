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

	static bool m_Begin;		//����N���p
	static bool m_DebugMode;	//�f�o�b�O���[�h����
	static int  m_Coin;			//�R�C���̈��p��
	static int  m_PlColor;		//�r�J���[
	static int  m_PlClown;		//�r����

	int m_SeepDropTime = 0;

	//�֐�
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