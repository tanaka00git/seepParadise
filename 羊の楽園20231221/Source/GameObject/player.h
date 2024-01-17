#pragma once
#include "..\GameObject\gameObject.h"

enum class PLAYER_STATE
{
	NORMAL,
	DASH,
	DEATH,
	UNUSED,
};

class Player : public GameObject
{
private:
	PLAYER_STATE m_PlayerState = PLAYER_STATE::NORMAL;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	D3DXMATRIX	m_ViewMatrix{};

	//�ǂݍ���
	class Shadow*	m_Shadow{};
	class HpBarS*	m_HpBarS{};
	static int		m_PlColor;		//�r�̐F
	static int		m_PlClown;		//����
	static class Model* m_Model;
	static class Model* m_ModelClown;
	static class Audio* m_ShotSE;
	static class Audio* m_WalkSE;
	static class Audio* m_DamageSE;
	static class Audio* m_DashSE;

	//�����o�ϐ�
	static int	m_DebugMode;											//�f�o�b�O���[�h����
	D3DXCOLOR	m_Color			 = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�z�F
	bool		m_TextureEnable  = true;								//�e�N�X�`����`�悷�邩
	D3DXVECTOR3 m_Velocity		 = D3DXVECTOR3(0.0f,0.0f,0.0f);			//���x
	D3DXVECTOR3 m_OriginalScale  = D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//�L�����̃T�C�Y
	int			m_InvincibleTime = 0;									//���G����

	bool  m_DashInit = false;	//�_�b�V������
	bool  m_Dash;				//�_�b�V���p
	int   m_Combo = 0;			//�R���{��
	int   m_ComboWait = 0;		//�R���{��\�����鎞��

	float m_Death = 0.14f;		//���S����܂ł̎���
	int   m_AttackStopTime = 0;

	int   m_Charge = 0;
	int   m_OldCharge = 0;
	int   m_FullCharge = 2000;

	bool m_TutorialEnd = false;		//�`���[�g���A���I������H
	bool m_DeleteInit = false;		//���S������
	bool m_AnimePause = true;		//�A�j���̌����؂�ւ�
	int  m_AnimeTime = 0;			//�A�j���̌����ύX�̎���
	int  m_WalkEffectTime = 0;		//�G�t�F�N�g�𔭐�������Ԋu����
	int	 m_DamageFlashTime = 0;		//�t���b�V���𔭐������鎞��

	//�X�e�[�^�X�n
	float m_Speed = 6.5;
	int	  m_Eye = 0;
	int   m_Life = 3;
	int   m_FullLife = 3;
	D3DXVECTOR3 m_BarScale = D3DXVECTOR3(1.2f, 1.0f, 1.2f);		//HP�o�[�̃T�C�Y

	//�֐�
	void UpdateDeath();
	void UpdateNormal();
	void UpdateDash();
	void UpdateUnused();
	void Collision(float& groundHeight);
	void WalkEffect();
	void TutorialText();
	void MouseTargetMove();
	void InvincibleColor();
	void DamageFlash();
	void AttackStop();
	void Anime();

public:
	static void Load();
	static void Unload();

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	//�Z�b�g�֐�
	void SetDamageMove();
	void AddCharge(int charge) { m_Charge += charge; };
	void SetCharge(int charge) { m_Charge = charge; };
	void AddLife(int Life){m_Life += Life;}
	void AddSpeed(float Speed) { m_Speed += Speed; };
	void AddFullLife(int Life) { m_FullLife += Life; };
	void AddEye(int Eye) { m_Eye += Eye; };
	void SetAttackStop(int attackStop) { m_AttackStopTime = attackStop; };
	void AddCombo(int combo) { m_Combo += combo; }

	//�擾�֐�
	float GetSpeed() const { return m_Speed; }
	int   GetCharge() const { return m_Charge; }
	int   GetFullCharge() const { return m_FullCharge; }
	bool  GetDash() const { return m_Dash; }
	int   GetFullLife() const { return m_FullLife; }
	int   GetLife() const { return m_Life; }
	int   GetAttackStop() const { return m_AttackStopTime; }
	int   GetEye() const { return m_Eye; }
	bool  GetDebug() const { return m_DebugMode; }
	int   GetInvincibleTime() const { return m_InvincibleTime; }
	PLAYER_STATE GetState() const { return m_PlayerState; }

	void SetColorClown(int color, int clown) { m_PlColor = color; m_PlClown = clown; };
	void SetDebugMode(bool debug) { m_DebugMode = debug; };
};