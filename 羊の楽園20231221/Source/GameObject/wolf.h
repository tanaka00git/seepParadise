#pragma once
#include "..\GameObject\characterObject.h"

enum class WOLF_STATE
{
	FREE,
	EATING,
	DAMAGE,
	SUPER_CHARGE,
	SUPER_ATTACK,
	TARGETING,
};

class Wolf : public CharacterObject
{
protected:
	WOLF_STATE m_WolfState = WOLF_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Model* m_Model;
	static class Model* m_ModelApple;
	static class Audio* m_SE_Eat;
	static class Audio* m_SE_Kick;
	static class Audio* m_SE_Critical;
	class AttackMarker* m_AttackMarker{};

	//�����o�ϐ�
	D3DXVECTOR3 m_PLForward{};			 //�v���C���[�̌���

	int     m_DaathTime			= 1400;	 //��������
	bool	m_AnimePause		= true;
	int		m_AnimeTime			= 0;
	float	m_AnimeRotationX	= 0.0f;	//�A�j���̕\���̌X��(���ۂ̌����Ƃ͈Ⴄ)
	float   m_OrientationTime	= 0.0f;	//�����ύX����
	float   m_NextRotTime		= 0.0f;	//���Ɍ����ύX����܂�
	int     m_NextRot			= 1;	//���̌���
	int	    m_EatStop			= 0;
	float	m_DeathStaging		= 0.14f;
	int		m_KnockBackTime		= 0;
	int		m_StunTime			= 0;
	int		m_StanGuardCount	= 0;	//�X�^�����肷�邽�߂̍U�����ꂽ��
	bool	m_DeleteInit		= false;

	//�X�e�[�^�X
	bool	m_Item				= false;						//�A�C�e���������ʗp
	int		m_CoinDrop			= 2;							//�R�C���h���b�v��
	int		m_StanGuard			= 1;							//�X�^���ɂȂ�܂łɕK�v�ȍU����
	int		m_BiteCount			= 1;							//�𚐉�
	D3DXVECTOR3 m_BarScale = D3DXVECTOR3(0.7f,0.7f,0.7f);		//HP�o�[�̃T�C�Y
	float m_HpBarPosY			= 1.8f;							//HP�o�[��Y���W
	float m_Tracking			= 5.0f;							//�ǔ��͈�

	//�֐�
	void UpdateAlive() override;
	void UpdateDead() override;
	void UpdateFree();
	virtual void UpdateTargeting();
	void UpdateDelete();
	void UpdateEating();
	void UpdateDamage();
	virtual void UpdateSuperCharge() {};
	virtual void UpdateSuperAttack() {};
	void Collision(float& groundHeight)override;
	void KnockBack();
	void Anime();

public:
	static void Load();
	static void Unload();
	void Init()override;
	void Uninit()override;
	void Update()override;
	virtual void Draw()override;

	void SetDaathTimeDelete() { m_DaathTime = 0; };
	void SetWolfState(WOLF_STATE wolfState) { m_WolfState = wolfState; }
	void SetDamageMove() override;
	void SetPosEnemyData(D3DXVECTOR3 position, int num);
	virtual void SetEnemyData(int data);
};