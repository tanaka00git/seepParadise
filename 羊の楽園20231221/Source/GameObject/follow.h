#pragma once
#include "..\GameObject\gameObject.h"

enum class FOLLOW_STATE
{
	FREE,
	NORMAL,
	DASH,
	DEATH,
};

class Follow : public GameObject
{
private:
	FOLLOW_STATE m_FollowState = FOLLOW_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//�ǂݍ���
	static class Model* m_Model;
	static class Audio* m_SE_Follow;
	static class Audio* m_SE_Release;
	class Shadow* m_Shadow{};

	//�����o�ϐ�
	D3DXVECTOR3 m_Velocity		= D3DXVECTOR3(0.0f,0.0f,0.0f);		 //���x
	D3DXCOLOR   m_Color			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //�J���[
	bool	    m_TextureEnable = true;								 //�e�N�X�`���K�p���邩

	int	  m_AttackStopTime	    = 0;	 //�U�����~���鎞��
	int	  m_DaathTime		    = 200;	 //��������
	int	  m_OrientationTime	    = 0;	 //�����ύX����
	int	  m_NextOrientationTime = 0;	 //���Ɍ����ύX����܂�
	int	  m_NextRot			    = 1;	 //���̌���

	int	  m_AgainFollow			= 0;	 //�ēx���ԂɂȂ��܂ł̎���

	int   m_AnimeTime			= 0;	 //�A�j���̌����̎���
	bool  m_AnimePause			= true;  //�A�j���̌����̔���
	float m_Death				= 0.14f; //���S����܂ł̎���
	int   m_WalkEffectTime		= 0;	 //���G�t�F�N�g���o��܂ł̎���
	int	  m_DamageFlashTime		= 0;	 //�_���[�W�Ńt���b�V�����鎞��

	//�X�e�[�^�X
	int   m_Life				= 1;
	float m_Speed				= 6.5f;

	//�֐�
	void UpdateFree();
	void UpdateNormal();
	void UpdateDash();
	void UpdateDeath();
	void Collision(float& groundHeight);
	void OrtOrientationChange();
	void WalkEffect();
	void PlayerTracking();
	void AttackStop();
	void Anime();
	void DamageFlash();

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	FOLLOW_STATE GetState() { return m_FollowState; }
	int GetAttackStop() { return m_AttackStopTime; }

	void SetAttackStop(int attackStop) { m_AttackStopTime = attackStop; };
	void AddLife(int Life){m_Life += Life;}
	void SetState(FOLLOW_STATE followState) { m_FollowState = followState; }
};