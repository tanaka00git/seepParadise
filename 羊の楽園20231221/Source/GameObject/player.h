#pragma once
#include "..\GameObject\gameObject.h"

enum class PLAYER_STATE
{
	NORMAL,
	DASH,
	DEATH,
};

class Player : public GameObject
{
private:
	PLAYER_STATE m_PlayerState = PLAYER_STATE::NORMAL;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};
	D3DXMATRIX	m_ViewMatrix{};

	//読み込み
	class Audio* m_ShotSE{};
	class Audio* m_WalkSE{};
	class Audio* m_DamageSE{};
	class Audio* m_ChargeSE{};
	class Audio* m_DashSE{};
	class Shadow* m_Shadow{};
	class HpBarS* m_HpBarS{};
	static int m_PlColor;		//羊の色
	static int m_PlClown;		//王冠
	static class Model* m_Model;
	static class Model* m_ModelClown;

	//メンバ変数
	static int m_DebugMode;		//デバッグモード判定
	D3DXVECTOR3 m_Velocity{};	//速度
	bool m_Use;
	int m_InvincibleTime = -1;	//無敵時間
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bool m_TextureEnable = true;

	bool m_DashInit = false;	//ダッシュ初回時
	bool m_Dash;				//ダッシュ用
	int  m_ComboWait = 0;		//コンボを表示する時間

	float m_Death = 0.14f;		//死亡するまでの時間
	int m_time = 0;
	int m_AttackStop = 0;

	int m_Combo = 0;
	int m_Charge = 0;
	int  m_OldCharge = 0;
	int m_FullCharge = 2000;

	bool m_ChargeFlagSE = false;	//チャージフラグ時のSE初回
	bool m_TutorialEnd = false;		//チュートリアル終わった？
	bool m_DeleteInit = false;		//死亡時初回
	bool m_Pause = true;
	bool m_DamageMove = false;
	int m_WalkEffectTime = 0;
	int	 m_DamageFlashTime = 0;

	//ステータス系
	float m_Speed = 6.5;
	int	  m_Eye = 0;
	int   m_Life = 3;
	int   m_FullLife = 3;
	D3DXVECTOR3 m_BarScale = D3DXVECTOR3(1.2f, 1.0f, 1.2f);		//HPバーのサイズ
	float m_ShadowSC = 1.0f;

public:
	static void Load();
	static void Unload();

	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	
	void UpdateDeath();
	void UpdateNormal();
	void UpdateDash();
	void Collision(float &groundHeight);
	void WalkEffect();
	void TutorialText();
	void MouseTargetMove();
	void InvincibleColor();
	void DamageFlash();
	void AttackStop();
	void Anime();
	//セット関数
	void SetDamageMove();
	void AddCharge(int charge) { m_Charge += charge; };
	void SetCharge(int charge) { m_Charge = charge; };
	void AddLife(int Life){m_Life += Life;}
	void AddSpeed(float Speed) { m_Speed += Speed; };
	void AddFullLife(int Life) { m_FullLife += Life; };
	void AddEye(int Eye) { m_Eye += Eye; };
	void SetAttackStop(int attackStop) { m_AttackStop = attackStop; };
	void AddCombo(int combo) { m_Combo += combo; }

	//取得関数
	float GetSpeed() { return m_Speed; }
	int GetCharge() { return m_Charge; }
	int GetFullCharge() { return m_FullCharge; }
	bool GetDash() { return m_Dash; }
	bool GetUse() { return m_Use; }
	int GetFullLife() { return m_FullLife; }
	int GetLife() { return m_Life; }
	int GetAttackStop() { return m_AttackStop; }
	int GetEye() { return m_Eye; }
	bool GetDebug() { return m_DebugMode; }
	PLAYER_STATE GetState() { return m_PlayerState; }
	int GetInvincibleTime() { return m_InvincibleTime; }

	void SetColorClown(int color, int clown) { m_PlColor = color; m_PlClown = clown; };
	void SetDebugMode(bool debug) { m_DebugMode = debug; };
};