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
	D3DXMATRIX  m_ProjectionMatrix{};

	//�����o�ϐ�
	float m_ShakeAmplitudeX = 0.0f;
	float m_ShakeAmplitudeY = 0.0f;
	float m_ShakeTime	   = 0.0f;
	float m_ShakeOffsetX = 0.0f;
	float m_ShakeOffsetY = 0.0f;

	//�֐�
	void UpdateTitle();
	void UpdateGame();
	void UpdateResult();

public:
	void Init() override;
	void Update() override;
	void Draw() override;

	//�Z�b�g�֐�
	void SetShake(float AmplitudeX, float AmplitudeY)
	{
		m_ShakeAmplitudeX = AmplitudeX;
		m_ShakeAmplitudeY = AmplitudeY;
	}
	void SetCameraState(CAMERA_STATE state) { m_CameraState = state; }
	
	//�Q�b�g�֐�
	bool CheckView(D3DXVECTOR3 position, D3DXVECTOR3 scale);
	D3DXMATRIX	GetViewMatrix() const { return m_ViewMatrix; };
	D3DXMATRIX	GetProjectionMatrix() const { return m_ProjectionMatrix; };
};