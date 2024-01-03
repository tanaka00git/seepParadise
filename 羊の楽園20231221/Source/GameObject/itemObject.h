#pragma once
#include "..\GameObject\gameObject.h"

enum class ITEM_STATE
{
	NORMAL,
	DEATH,
};

class ItemObject : public GameObject
{
protected:
	ITEM_STATE m_ItemState = ITEM_STATE::NORMAL;

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Shadow* m_Shadow{};

	//ƒƒ“ƒo•Ï”
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
	int  m_DaathTime  = 500;
	bool m_DeleteInit = false;	//Á‹‰Šú‰»
	bool m_DropJump   = false;	//ƒWƒƒƒ“ƒv‚·‚é

	//ŠÖ”
	virtual void UpdateMove();
	virtual void UpdateDelete();
	virtual void MoveGet() {};

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	virtual void SetDrop();
};