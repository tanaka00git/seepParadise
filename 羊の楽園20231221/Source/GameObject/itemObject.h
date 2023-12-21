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
	D3DXVECTOR3 m_Velocity{};

	class Shadow* m_Shadow{};

	int  m_DaathTime = 500;
	bool m_DeleteInit = false;
	bool m_DropJump = false;

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	virtual void UpdateMove();
	virtual void UpdateDelete();
	virtual void MoveGet() {};

	virtual void SetDrop();
};