#pragma once
#include "..\GameObject\characterObject.h"

enum class FOLLOW_STATE
{
	FREE,
	NORMAL,
	DASH,
};

class Follow : public CharacterObject
{
private:
	FOLLOW_STATE m_FollowState = FOLLOW_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//�ǂݍ���
	static class Model* m_Model;
	static class Audio* m_SE_Follow;
	static bool m_SE_FollowCheck;
	static class Audio* m_SE_Release;

	//�����o�ϐ�
	int	  m_AttackStopTime	    = 0;	 //�U�����~���鎞��
	int	  m_DaathTime		    = 200;	 //��������
	int	  m_OrientationTime	    = 0;	 //�����ύX����
	int	  m_NextOrientationTime = 0;	 //���Ɍ����ύX����܂�
	int	  m_NextRot			    = 1;	 //���̌���
	int	  m_AgainFollow			= 0;	 //�ēx���ԂɂȂ��܂ł̎���
	int   m_AnimeTime			= 0;	 //�A�j���̌����̎���
	bool  m_AnimePause			= true;  //�A�j���̌����̔���
	float m_AnimeRotationX		= 0.0f;	 //�A�j���̕\���̌X��(���ۂ̌����Ƃ͈Ⴄ)
	float m_DeathStaging		= 0.14f; //���S����܂ł̎���
	int   m_WalkEffectTime		= 0;	 //���G�t�F�N�g���o��܂ł̎���

	//�֐�
	void UpdateAlive() override;
	void UpdateDead() override;
	void UpdateFree();
	void UpdateNormal();
	void UpdateDash();
	void Collision(float& groundHeight);
	void OrtOrientationChange();
	void WalkEffect();
	void PlayerTracking();
	void AttackStop();
	void Anime();

public:
	static void Load();
	static void Unload();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	FOLLOW_STATE GetState() { return m_FollowState; }
	int GetAttackStop() { return m_AttackStopTime; }

	void SetKnockBack();
	void SetState(FOLLOW_STATE followState) { m_FollowState = followState; }
};