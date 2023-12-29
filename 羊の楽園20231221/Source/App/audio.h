#pragma once
#include <xaudio2.h>
#include "..\GameObject\gameObject.h"

class Audio : public Component
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};

	float m_TargetVolume;   // フェード先の音量
	float m_CurrentVolume;  // 現在の音量
	float m_FadeSpeed;      // フェードの速さ


public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();

	void Load(const char *FileName);
	void Play(float Volume, bool Loop = false);
	void Volume(float Volume);
	// 音量を指定した値にフェードさせるメソッド
	void FadeUpdate();
	void FadeToVolume(float targetVolume, float fadeSpeed);
};

