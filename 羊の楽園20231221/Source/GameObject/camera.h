#pragma once
#include "..\GameObject\gameObject.h"

enum class CAMERA_STATE
{
	TITLE,
	GAME,
	RESULT,
};

class Camera : public GameObject
{
private:
	CAMERA_STATE m_CameraState = CAMERA_STATE::TITLE;
	D3DXVECTOR3	m_Target{};
	D3DXMATRIX	m_ViewMatrix{};
	D3DXMATRIX  m_ProjectionMateix{};

	float m_ShakeAmplitude{};
	float m_ShakeTime{};
	float m_ShakeOffset{};

public:
	void Init() override;
	void Update() override;
	void Draw() override;

	void SetCameraState(CAMERA_STATE state) { m_CameraState = state; }
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);

	D3DXMATRIX	GetViewMatrix() { return m_ViewMatrix; };
	D3DXMATRIX	GetProjectionMateix() { return m_ProjectionMateix; };

	void UpdateTitle();
	void UpdateGame();
	void UpdateResult();

	void SetShake(float Amplitude) { m_ShakeAmplitude = Amplitude; }
};