#pragma once
#include "..\GameObject\gameObject.h"

class Spring : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Model* m_Model;

	int			m_DaathTime = 3000;		//自動消滅時間
	D3DXVECTOR3 m_OriginalScale = D3DXVECTOR3(2.5f, 1.0f, 2.5f);	//本来のスケール
	void SmoothAppearance(bool growing);

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};