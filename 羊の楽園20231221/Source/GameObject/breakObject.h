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
	D3DXVECTOR3 m_Velocity   = D3DXVECTOR3(0.0f,0.0f,0.0f); //速度
	bool		m_DeleteInit = false;						//削除時初回
	int			m_DaathTime  = 3000;						//自動消滅時間

	//ステータス
	int			m_FullLife	 = 7;							//最大体力
	int			m_Life		 = 7;							//体力

	//関数
	virtual void UpdateMove();
	virtual void UpdateDelete();
	virtual void UpdateDeath();

public:
	static void Load();
	static void Unload();

	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void SetDamageMove();
	void LifeBarMove();
	BREAKOBJECT_STATE GetState() { return m_BreakObjectState; }
};