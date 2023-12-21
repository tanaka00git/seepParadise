#pragma once
#include "..\GameObject\gameObject.h"

class Cylinder : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	static class Model* m_Model;

public:
	static void Load();
	static void Unload();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};