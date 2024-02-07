#pragma once
#include "..\GameObject\characterObject.h"

enum class FOLLOW_STATE
{
	FREE,
	NORMAL,
	DASH,
};

class Follow : public CharacterObject
{
private:
	FOLLOW_STATE m_FollowState = FOLLOW_STATE::FREE;
	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	//読み込み
	static class Model* m_Model;
	static class Audio* m_SE_Follow;
	static bool m_SE_FollowCheck;
	static class Audio* m_SE_Release;

	//メンバ変数
	int	  m_AttackStopTime	    = 0;	 //攻撃を停止する時間
	int	  m_DaathTime		    = 200;	 //自動消滅
	int	  m_OrientationTime	    = 0;	 //向き変更時間
	int	  m_NextOrientationTime = 0;	 //次に向き変更するまで
	int	  m_NextRot			    = 1;	 //次の向き
	int	  m_AgainFollow			= 0;	 //再度仲間になれるまでの時間
	int   m_AnimeTime			= 0;	 //アニメの向きの時間
	bool  m_AnimePause			= true;  //アニメの向きの判定
	float m_AnimeRotationX		= 0.0f;	 //アニメの表示の傾き(実際の向きとは違う)
	float m_DeathStaging		= 0.14f; //死亡するまでの時間
	int   m_WalkEffectTime		= 0;	 //煙エフェクトが出るまでの時間

	//関数
	void UpdateAlive() override;
	void UpdateDead() override;
	void UpdateFree();
	void UpdateNormal();
	void UpdateDash();
	void Collision(float& groundHeight);
	void OrtOrientationChange();
	void WalkEffect();
	void PlayerTracking();
	void AttackStop();
	void Anime();

public:
	static void Load();
	static void Unload();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	FOLLOW_STATE GetState() { return m_FollowState; }
	int GetAttackStop() { return m_AttackStopTime; }

	void SetKnockBack();
	void SetState(FOLLOW_STATE followState) { m_FollowState = followState; }
};