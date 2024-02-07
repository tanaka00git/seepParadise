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

	//�����o�ϐ�
	D3DXVECTOR3 m_Velocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 m_OriginalScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	//�L�����̃T�C�Y
	int  m_DaathTime  = 500;
	bool m_DeleteInit = false;	//������������
	bool m_DropJump   = false;	//�W�����v����

	//�֐�
	virtual void UpdateMove();
	virtual void UpdateDelete();
	virtual void MoveGet() {};
	void SmoothAppearance(bool growing);

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw() {};

	virtual void SetDrop();
};