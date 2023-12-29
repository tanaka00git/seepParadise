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

	float m_TargetVolume;   // �t�F�[�h��̉���
	float m_CurrentVolume;  // ���݂̉���
	float m_FadeSpeed;      // �t�F�[�h�̑���


public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();

	void Load(const char *FileName);
	void Play(float Volume, bool Loop = false);
	void Volume(float Volume);
	// ���ʂ��w�肵���l�Ƀt�F�[�h�����郁�\�b�h
	void FadeUpdate();
	void FadeToVolume(float targetVolume, float fadeSpeed);
};

