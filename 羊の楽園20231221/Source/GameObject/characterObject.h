#pragma once
#include "..\GameObject\gameObject.h"

enum class CHARACTER_STATE
{
	ALIVE,
	DEAD,
	UNUSED,
};

class CharacterObject : public GameObject
{
protected:
	CHARACTER_STATE m_CharacterState = CHARACTER_STATE::ALIVE;

	//�ǂݍ���
	class Shadow* m_Shadow{};
	class HpBarS* m_HpBarS{};

	//�����o�ϐ�
	D3DXCOLOR	m_Color				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�z�F
	bool		m_TextureEnable		= true;									//�e�N�X�`����`�悷�邩
	D3DXVECTOR3 m_Velocity			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���x
	D3DXVECTOR3 m_OriginalScale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//�L�����̃T�C�Y
	int			m_DamageFlashTime	= 0;									//�t���b�V���𔭐������鎞��
	D3DXVECTOR3 m_BarScale			= D3DXVECTOR3(1.2f, 1.0f, 1.2f);		//HP�o�[�̃T�C�Y

	//�X�e�[�^�X
	int   m_Life		= 1;	//�̗�
	int   m_FullLife	= 1;	//�ő�̗�
	float m_Speed		= 6.5;	//���x

	//�֐�
	virtual void UpdateAlive();
	virtual void UpdateDead() {};
	virtual void UpdateUnused();
	virtual void DamageFlash();
	virtual void Collision(float& groundHeight);

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw() {};

	//�Z�b�g�֐�
	virtual void SetDamageMove();
	void SetDrop();
	void AddLife(int life) { m_Life += life; }

	//�Q�b�g�֐�
	CHARACTER_STATE GetCharacterState() const { return m_CharacterState; }
};