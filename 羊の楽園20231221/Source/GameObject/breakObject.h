#pragma once
#include "..\GameObject\gameObject.h"

enum class BREAKOBJECT_STATE
{
	NORMAL,
	DEATH,
	ORTDEATH,
};

class BreakObject : public GameObject
{
protected:
	BREAKOBJECT_STATE m_BreakObjectState = BREAKOBJECT_STATE::NORMAL;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class HpBarS* m_HpBarS{};
	static class Audio* m_SE_Kick;

	//�����o�ϐ�
	D3DXCOLOR	m_Color				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//�z�F
	bool		m_TextureEnable		= true;								//�e�N�X�`���`��
	D3DXVECTOR3 m_Velocity			= D3DXVECTOR3(0.0f,0.0f,0.0f);		//���x
	D3DXVECTOR3 m_OriginalScale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�{���̃X�P�[��
	bool		m_DeleteInit		= false;							//�폜������
	int			m_DaathTime			= 2000;								//�������Ŏ���
	int			m_DamageFlashTime	= 0;								//�_���[�W�̎���

	//�X�e�[�^�X
	int			m_FullLife	= 7;									//�ő�̗�
	int			m_Life		= 7;									//�̗�
	D3DXVECTOR3 m_BarScale	= D3DXVECTOR3(1.2f, 0.8f, 0.8f);		//HP�o�[�̃T�C�Y
	float		m_HpBarPosY	= 1.5f;									//HP�o�[��Y���W

	//�֐�
	virtual void UpdateMove();
	virtual void UpdateDelete();
	virtual void UpdateDead();
	virtual void Collision(float& groundHeight);

public:
	static void Load();
	static void Unload() {};

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw() {};

	void DamageFlash();
	void SetDamageMove();
	void LifeBarMove();
	BREAKOBJECT_STATE GetState() { return m_BreakObjectState; }
};