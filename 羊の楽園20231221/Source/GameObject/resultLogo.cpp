#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\resultLogo.h"
#include "..\App\sprite.h"

const float ALPHA_VALUE = 0.03f;

void ResultLogo::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(730.0f, 485.0f, 240, 40, "asset\\texture\\titlePoint2.png");
	AddComponent<Sprite>()->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\result.png");
}

void ResultLogo::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void ResultLogo::Update()
{
	GameObject::Update();

	//������������
	if (m_UpDown) {
		m_Alpha += ALPHA_VALUE;
		if (m_Alpha > 1.0f) { m_UpDown = false; }
	}
	else {
		m_Alpha -= ALPHA_VALUE;
		if (m_Alpha < 0.5f) { m_UpDown = true; }
	}
	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
}

void ResultLogo::Draw()
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