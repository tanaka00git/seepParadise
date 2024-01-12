#pragma once
#include "..\GameObject\gameObject.h"

enum class WOLF_STATE
{
	FREE,
	EATING,
	DAMAGE,
	TARGETING,
	DEATH,
};

class Wolf : public GameObject
{
private:
	WOLF_STATE m_WolfState = WOLF_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};


	static class Model* m_Model;
	static class Model* m_Model2;
	static class Model* m_Model3;
	static class Audio* m_SE_Eat;
	static class Audio* m_SE_Kick;
	class Shadow* m_Shadow{};
	class HpBarS* m_HpBarS{};

	//�����o�ϐ�
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //���x
	D3DXVECTOR3 m_PLForward{};								//�v���C���[�̌���

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bool	  m_TextureEnable = true;
	int		  m_ColorChange = 0;

	int     m_DaathTime = 1400;	 //��������
	
	bool	m_AnimePause = true;
	int		m_AnimeTime = 0;
	float   m_OrientationTime = 0.0f;	 //�����ύX����
	float   m_NextRotTime = 0.0f;		 //���Ɍ����ύX����܂�
	int     m_NextRot = 1;			     //���̌���
	int	    m_EatStop = 0;
	float	m_Death = 0.14f;
	int		m_KnockBackTime = 0;
	int		m_StunTime = 0;
	bool	m_DeleteInit = false;

	//�X�e�[�^�X
	int		m_Data = 2;
	bool	m_Item = false;
	int		m_Life = 3;
	int		m_FullLife = 3;
	float	m_Speed = 0.02f;									//���x
	int		m_CoinDrop = 2;										//�R�C���h���b�v��
	D3DXVECTOR3 m_CharaScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�L�����̃T�C�Y
	D3DXVECTOR3 m_BarScale = D3DXVECTOR3(0.7f,0.7f,0.7f);		//HP�o�[�̃T�C�Y
	float m_HpBarPosY = 1.8f;									//HP�o�[��Y���W
	float m_Tracking = 5.0f;									//�ǔ��͈�
	bool  m_Disaster = false;									//��΂��z
	int   m_DisasterCount = 0;									//��΂��z

	//�֐�
	void UpdateFree();
	void UpdateTargeting();
	void UpdateDelete();
	void UpdateEating();
	void UpdateDeath();
	void UpdateDamage();
	void Collision(float& groundHeight);
	void KnockBack();
	void DisasterMove();
	void DamageFlash();
	void Anime();

public:
	static void Load();
	static void Unload();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void AddLife(int life) { m_Life += life; }
	void SetDamageMove();
	void SetPosEnemyData(D3DXVECTOR3 Position, int Num);
	void SetEnemyData(int data);
};