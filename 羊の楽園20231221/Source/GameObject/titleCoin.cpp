#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\titleCoin.h"
#include "..\App\sprite.h"
#include "..\App\input.h"

#define SCALE 30.0f
#define GRAVITY 0.15f

void TitleCoin::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	// �}�E�X�̍��W���擾
	POINT mousePos = Input::GetMousePosition();
	m_Position = D3DXVECTOR3(static_cast<float>(mousePos.x) - SCALE / 2, static_cast<float>(mousePos.y) - SCALE / 2, 0.0f - SCALE / 2);

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(m_Position.x, m_Position.y, SCALE, SCALE, "asset\\texture\\coin.png");
	m_Sprite->SetPosition(m_Position);

	//���
	m_Velocity.x = frand() * 5.0f - 2.5f;
	m_Velocity.y = -3;
}

void TitleCoin::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}


void TitleCoin::Update()
{
	GameObject::Update();

	//�d��
	m_Velocity.y += GRAVITY;

	if (m_Velocity.y >= 0)
	{
		m_Alpha -= 0.01f;
	}
	if (m_Alpha <= 0.0f)
	{
		SetDestroy();
	}
	m_Position += m_Velocity;
	m_Sprite->SetPosition(m_Position);
	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}


void TitleCoin::Draw()
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