#pragma once
#include "..\GameObject\characterObject.h"

enum class WOLF_STATE
{
	FREE,
	EATING,
	DAMAGE,
	TARGETING,
};

class Wolf : public CharacterObject
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

	//メンバ変数
	D3DXVECTOR3 m_PLForward{};										//プレイヤーの向き
	int		m_ColorChange = 0;

	int     m_DaathTime = 1400;	 //自動消滅
	
	bool	m_AnimePause = true;
	int		m_AnimeTime = 0;
	float	m_AnimeRotationX = 0.0f;	//アニメの表示の傾き(実際の向きとは違う)
	float   m_OrientationTime = 0.0f;	//向き変更時間
	float   m_NextRotTime = 0.0f;		//次に向き変更するまで
	int     m_NextRot = 1;				//次の向き
	int	    m_EatStop = 0;
	float	m_Death = 0.14f;
	int		m_KnockBackTime = 0;
	int		m_StunTime = 0;
	bool	m_DeleteInit = false;

	//ステータス
	int		m_Data = 2;
	bool	m_Item = false;
	int		m_CoinDrop = 2;										//コインドロップ数
	D3DXVECTOR3 m_BarScale = D3DXVECTOR3(0.7f,0.7f,0.7f);		//HPバーのサイズ
	float m_HpBarPosY = 1.8f;									//HPバーのY座標
	float m_Tracking = 5.0f;									//追尾範囲
	bool  m_Disaster = false;									//やばい奴
	int   m_DisasterCount = 0;									//やばい奴

	//関数
	void UpdateAlive() override;
	void UpdateDeath() override;
	void UpdateFree();
	void UpdateTargeting();
	void UpdateDelete();
	void UpdateEating();
	void UpdateDamage();
	void Collision(float& groundHeight)override;
	void KnockBack();
	void DisasterMove();
	void DamageFlash();
	void Anime();

public:
	static void Load();
	static void Unload();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void AddLife(int life) { m_Life += life; }
	void SetDamageMove();
	void SetPosEnemyData(D3DXVECTOR3 position, int num);
	void SetEnemyData(int data);
};