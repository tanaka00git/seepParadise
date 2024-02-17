#pragma once
#include "..\GameObject\characterObject.h"

enum class HUMAN_STATE
{
	FREE,
	MAKING,
	DAMAGE,
	NIGHT,
	TARGETING,
};

class Human : public CharacterObject
{
protected:
	HUMAN_STATE m_HumanState = HUMAN_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Model* m_Model;
	static class Audio* m_SE_Kick;
	static class Audio* m_SE_Critical;
	static class Audio* m_SE_Make;

	//�����o�ϐ�
	D3DXVECTOR3 m_PLForward{};			//�v���C���[�̌���

	int     m_DaathTime			= 1400;	//��������
	bool	m_AnimePause		= true;
	int		m_AnimeTime			= 0;
	float	m_AnimeRotationX	= 0.0f;	//�A�j���̕\���̌X��(���ۂ̌����Ƃ͈Ⴄ)
	float   m_OrientationTime	= 0.0f;	//�����ύX����
	float   m_NextRotTime		= 0.0f;	//���Ɍ����ύX����܂�
	int     m_NextRot			= 1;	//���̌���
	int	    m_MakingTime		= 0;	//�����鎞��
	float	m_DeathStaging		= 0.14f;
	int		m_KnockBackTime		= 0;	//�m�b�N�o�b�N�̎���
	int		m_StunTime			= 0;	//�X�^���̎���
	bool	m_DeleteInit		= false;//�f���[�g����Init����
	int		m_MakingInvalidTime	= 0;	//���C�L���O�ł��Ȃ�����

	//�X�e�[�^�X
	int		m_FeetDrop		= 4;									//�R�C���h���b�v��
	D3DXVECTOR3 m_BarScale	= D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//HP�o�[�̃T�C�Y
	float m_HpBarPosY		= 1.8f;									//HP�o�[��Y���W
	float m_Tracking		= 10.0f;								//�ǔ��͈�

	//�֐�
	void UpdateAlive() override;
	void UpdateDead() override;
	void UpdateFree();
	void UpdateTargeting();
	void UpdateDelete();
	void UpdateMaking();
	void UpdateDamage();
	void UpdateNight();
	void Collision(float& groundHeight)override;
	void Escape();
	void KnockBack();
	void Anime();
	virtual void Make(D3DXVECTOR3 objectPosition);

public:
	static void Load();
	static void Unload();
	void Init()override;
	void Uninit()override;
	void Update()override;
	virtual void Draw()override;

	void AddLife(int life) { m_Life += life; }
	void SetDamageMove() override;
};