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
private:
	HUMAN_STATE m_HumanState = HUMAN_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Model* m_Model;
	static class Audio* m_SE_Kick;
	static class Audio* m_SE_Make;

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
	int	    m_MakingTime = 0;
	float	m_Death = 0.14f;
	int		m_KnockBackTime = 0;
	int		m_StunTime = 0;
	bool	m_DeleteInit = false;

	//ステータス
	bool	m_Item = false;
	int		m_CoinDrop = 4;										//コインドロップ数
	D3DXVECTOR3 m_BarScale = D3DXVECTOR3(1.0f, 1.5f, 1.0f);		//HPバーのサイズ
	float m_HpBarPosY = 1.8f;									//HPバーのY座標
	float m_Tracking = 10.0f;									//追尾範囲

	//関数
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

public:
	static void Load();
	static void Unload();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void AddLife(int life) { m_Life += life; }
	void SetDamageMove();
};