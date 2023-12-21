#pragma once
#include "..\GameObject\gameObject.h"

class ResultSeep : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	D3DXVECTOR3 m_Velocity{};//‘¬“x
	static class Model* m_Model;

	int m_Speed = 7;
	float m_time2 = 0.0f;
	bool m_Stop = false;
	float m_time = 0.0f;
	bool m_Pause = true;
	int m_NextRot = 1;
	float m_NextRotTime = 0.0f;

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();

};