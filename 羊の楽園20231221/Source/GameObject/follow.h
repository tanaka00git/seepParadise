#pragma once
#include "..\GameObject\gameObject.h"

enum class FOLLOW_STATE
{
	FREE,
	NORMAL,
	DASH,
	DEATH,
};

class Follow : public GameObject
{
private:
	FOLLOW_STATE m_FollowState = FOLLOW_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//読み込み
	static class Model* m_Model;
	static class Audio* m_SE_Follow;
	static class Audio* m_SE_Release;
	class Shadow* m_Shadow{};

	//メンバ変数
	D3DXVECTOR3 m_Velocity{};//速度
	D3DXCOLOR m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	bool m_TextureEnable = true;

	int m_AttackStopTime = 0;
	int m_DaathTime = 200;			//自動消滅
	int m_OrientationTime = 0;		//向き変更時間
	int m_NextOrientationTime = 0;	//次に向き変更するまで
	int m_NextRot = 1;				//次の向き

	int m_AgainFollow = 0;	//再度仲間になれるまでの時間

	int   m_AnimeTime = 0;
	bool  m_AnimePause = true;
	float m_Death = 0.14f;
	int   m_WalkEffectTime = 0;
	int	  m_DamageFlashTime = 0;

	//ステータス
	int   m_Life = 1;
	float m_Speed = 6.5f;

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void UpdateFree();
	void UpdateNormal();
	void UpdateDash();
	void UpdateDeath();

	void Collision(float & groundHeight);
	void OrtOrientationChange();
	void WalkEffect();
	void PlayerTracking();
	void AttackStop();
	void Anime();
	void DamageFlash();

	FOLLOW_STATE GetState() { return m_FollowState; }
	int GetAttackStop() { return m_AttackStopTime; }

	void SetAttackStop(int attackStop) { m_AttackStopTime = attackStop; };
	void AddLife(int Life){m_Life += Life;}
	void SetState(FOLLOW_STATE followState) { m_FollowState = followState; }
};