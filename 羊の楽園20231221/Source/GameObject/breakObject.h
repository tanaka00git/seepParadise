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

	//メンバ変数
	D3DXCOLOR	m_Color				= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//配色
	bool		m_TextureEnable		= true;								//テクスチャ描画
	D3DXVECTOR3 m_Velocity			= D3DXVECTOR3(0.0f,0.0f,0.0f);		//速度
	D3DXVECTOR3 m_OriginalScale		= D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//本来のスケール
	bool		m_DeleteInit		= false;							//削除時初回
	int			m_DaathTime			= 2000;								//自動消滅時間
	int			m_DamageFlashTime	= 0;								//ダメージの時間

	//ステータス
	int			m_FullLife	= 7;									//最大体力
	int			m_Life		= 7;									//体力
	D3DXVECTOR3 m_BarScale	= D3DXVECTOR3(1.2f, 0.8f, 0.8f);		//HPバーのサイズ
	float		m_HpBarPosY	= 1.5f;									//HPバーのY座標

	//関数
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