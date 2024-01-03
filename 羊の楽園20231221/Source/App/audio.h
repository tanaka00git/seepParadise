#pragma once
#include <xaudio2.h>
#include "..\GameObject\gameObject.h"

class Audio : public Component
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice* m_SourceVoice{};
	BYTE* m_SoundData{};

	int	  m_Length{};
	int	  m_PlayLength{};

	float m_TargetVolume  = 0.0f;   // �t�F�[�h��̉���
	float m_CurrentVolume = 0.0f;   // ���݂̉���
	float m_FadeSpeed	  = 0.0f;   // �t�F�[�h�̑���

public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;
	void Uninit();
	void Load(const char *FileName);
	void Play(float Volume, bool Loop = false);
	void Volume(float Volume);

	void FadeUpdate();
	void FadeToVolume(float targetVolume, float fadeSpeed);
};

