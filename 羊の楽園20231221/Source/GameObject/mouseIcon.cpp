#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\mouseIcon.h"
#include "..\App\sprite.h"
#include "..\App\input.h"

#define SCALE 30.0f

void MouseIcon::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	// �}�E�X�̍��W���擾
	POINT mousePos = Input::GetMousePosition();
	m_Position = D3DXVECTOR3(static_cast<float>(mousePos.x) - SCALE / 2, static_cast<float>(mousePos.y) - SCALE / 2, 0.0f - SCALE / 2);

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(m_Position.x, m_Position.y, SCALE, SCALE, "asset\\texture\\mouse.png");
	m_Sprite->SetPosition(m_Position);
}

void MouseIcon::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}


void MouseIcon::Update()
{
	GameObject::Update();

	// �}�E�X�̍��W���擾
	POINT mousePos = Input::GetMousePosition();
	m_Position = D3DXVECTOR3(static_cast<float>(mousePos.x) - SCALE / 2, static_cast<float>(mousePos.y) - SCALE / 2, 0.0f - SCALE / 2);
	m_Sprite->SetPosition(m_Position);
}


void MouseIcon::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �g�p����V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���N���X���\�b�h�̌Ăяo��
	GameObject::Draw();
}