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

	//�����o�ϐ�
	float m_ShakeAmplitude = 0.0f;
	float m_ShakeTime	   = 0.0f;
	float m_ShakeOffset    = 0.0f;

	//�֐�
	void UpdateTitle();
	void UpdateGame();
	void UpdateResult();

public:
	void Init() override;
	void Update() override;
	void Draw() override;

	//�Z�b�g�֐�
	void SetShake(float Amplitude) { m_ShakeAmplitude = Amplitude; }
	void SetCameraState(CAMERA_STATE state) { m_CameraState = state; }
	
	//�Q�b�g�֐�
	bool CheckView(D3DXVECTOR3 Position, D3DXVECTOR3 Scale);
	D3DXMATRIX	GetViewMatrix() { return m_ViewMatrix; };
	D3DXMATRIX	GetProjectionMateix() { return m_ProjectionMateix; };
};