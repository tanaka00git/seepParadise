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

	//読み込み
	class Shadow* m_Shadow{};
	class HpBarS* m_HpBarS{};

	//メンバ変数
	D3DXCOLOR	m_Color				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//配色
	bool		m_TextureEnable		= true;									//テクスチャを描画するか
	D3DXVECTOR3 m_Velocity			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//速度
	D3DXVECTOR3 m_OriginalScale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);		//キャラのサイズ
	int			m_DamageFlashTime	= 0;									//フラッシュを発生させる時間
	D3DXVECTOR3 m_BarScale			= D3DXVECTOR3(1.2f, 1.0f, 1.2f);		//HPバーのサイズ

	//ステータス
	int   m_Life		= 1;	//体力
	int   m_FullLife	= 1;	//最大体力
	float m_Speed		= 6.5;	//速度

	//関数
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

	//セット関数
	virtual void SetDamageMove();
	void SetDrop();
	void AddLife(int life) { m_Life += life; }

	//ゲット関数
	CHARACTER_STATE GetCharacterState() const { return m_CharacterState; }
};