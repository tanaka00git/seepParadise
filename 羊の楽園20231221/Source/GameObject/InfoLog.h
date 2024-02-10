#pragma once
#include "..\GameObject\gameObject.h"

class InfoLog : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Sprite* m_Sprite{};

	D3DXVECTOR3 m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float		m_Alpha		 = 0.0f;
	int			m_DeleteTime = 0;
	bool		m_LogEnd     = false;

	int			m_LogNum     = 0;
	int			m_MoveNum    = 0;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetNum(int num, int moveNum, D3DXVECTOR3 position);
};