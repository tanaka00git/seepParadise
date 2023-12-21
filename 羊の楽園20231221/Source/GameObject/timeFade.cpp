#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\timeFade.h"
#include "..\App\sprite.h"

Sprite*TimeFade::m_Sprite{};
Sprite*TimeFade::m_Sprite2{};

void TimeFade::Load()
{
	m_Sprite = new Sprite();
	m_Sprite->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\fadeOver.png");

	m_Sprite2 = new Sprite();
	m_Sprite2->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\fadeSan.png");
}

void TimeFade::Unload()
{
}

void TimeFade::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void TimeFade::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}

void TimeFade::Update()
{
	GameObject::Update();
	
	//�t�F�[�h�C������
	if (!m_FadeOut)
	{

		m_Alpha -= 0.005f;
		m_Alpha2 += 0.005f;
		if (m_Alpha < 0.0f) {m_Alpha = 0.0f;}
		if (m_Alpha2 > 0.2f) {
			m_Alpha2 = 0.2f;
			m_FadeOut2 = m_FadeOut;
		}
	}
	else {
		m_Alpha += 0.005f;
		m_Alpha2 -= 0.005f;
		if (m_Alpha > 0.3f) {m_Alpha = 0.3f;}
		if (m_Alpha2 < 0.0f) {
			m_Alpha2 = 0.0f;
			m_FadeOut2 = m_FadeOut;

		}
	}

	m_Sprite->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.6f, m_Alpha));
	m_Sprite2->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha2));
}


void TimeFade::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �g�p����V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	m_Sprite->Draw();
	m_Sprite2->Draw();

	//���N���X���\�b�h�̌Ăяo��
	GameObject::Draw();
}