#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\titleLogo.h"
#include "..\App\renderer.h"
#include "..\App\sprite.h"

void TitleLogo::Init()
{

	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_SpriteTarget = AddComponent<Sprite>();
	m_SpriteTarget->Init(230.0f, 470.0f, 110, 30, "asset\\texture\\titlePoint.png");
	m_SpriteLogo = AddComponent<Sprite>();
	m_SpriteLogo->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\title.png");
	m_SpriteStart = AddComponent<Sprite>();
	m_SpriteStart->Init(230.0f, 470.0f, 110, 30, "asset\\texture\\titleStart.png");
	m_SpriteShop = AddComponent<Sprite>();
	m_SpriteShop->Init(430.0f, 470.0f, 110, 30, "asset\\texture\\titleShop.png");
	m_SpriteHelp = AddComponent<Sprite>();
	m_SpriteHelp->Init(630.0f, 470.0f, 110, 30, "asset\\texture\\titleHelp.png");
	m_SpritePressStart = AddComponent<Sprite>();
	m_SpritePressStart->Init(380.0f, 465.0f, 240, 40, "asset\\texture\\titlePress.png");

	m_SpriteHelp1Text = AddComponent<Sprite>();
	m_SpriteHelp1Text->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\help1.png");
	m_SpriteHelp2Text = AddComponent<Sprite>();
	m_SpriteHelp2Text->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\help2.png");

	m_SpriteShopText = AddComponent<Sprite>();
	m_SpriteShopText->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "asset\\texture\\shop.png");
	m_SpriteDebug = AddComponent<Sprite>();
	m_SpriteDebug->Init(0.0f, 0.0f, 300, 40, "asset\\texture\\timeLogDebug.png");

	m_SpriteStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_SpriteShop->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_SpriteHelp->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	m_SpriteHelp1Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_SpriteHelp2Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_SpriteShopText->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	m_SpritePressStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_SpriteTarget->SetPosition(D3DXVECTOR3(380.0f, 470.0f, 0.0f));
	m_SpriteTarget->SetScale(D3DXVECTOR3(240, 30, 0));

}

void TitleLogo::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}

void TitleLogo::Update()
{
	GameObject::Update();

	if (m_Select <= 0) { m_Select = 3; }
	else if (m_Select >= 4) { m_Select = 1; }

	if (m_Scene == 1)
	{
		m_Alpha7 -= 0.1f;
		m_SpriteTarget->SetScale(D3DXVECTOR3(0, 0, 0));

		//P�������Ă���������
		if (m_Alpha7 <= 0.0f)
		{
			m_Alpha7 = 0.0f;
			m_SpriteStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_SpriteShop->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteHelp->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteTarget->SetPosition(D3DXVECTOR3(230.0f, 470.0f, 0.0f));
			m_SpriteHelp1Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_SpriteHelp2Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_SpriteTarget->SetScale(D3DXVECTOR3(110, 30, 0));
			m_Scene = 2;
		}
	}
	else if (m_Scene == 2)
	{

		//�{�^���ʒu����
		if (m_Select == 1)
		{
			m_SpriteStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_SpriteShop->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteHelp->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteTarget->SetPosition(D3DXVECTOR3(230.0f, 470.0f, 0.0f));
			m_SpriteHelp1Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_SpriteHelp2Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else if (m_Select == 2)
		{
			m_SpriteStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteShop->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_SpriteHelp->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteTarget->SetPosition(D3DXVECTOR3(430.0f, 470.0f, 0.0f));
			m_SpriteHelp1Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_SpriteHelp2Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		}
		else if (m_Select == 3)
		{
			m_SpriteStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteShop->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteHelp->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_SpriteTarget->SetPosition(D3DXVECTOR3(630.0f, 470.0f, 0.0f));
			m_SpriteHelp1Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_SpriteHelp2Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else if (m_Select == 4)
		{
			m_SpriteStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteShop->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteHelp->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));
			m_SpriteTarget->SetPosition(D3DXVECTOR3(2000.0f, 470.0f, 0.0f));
			m_SpriteHelp1Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			m_SpriteHelp2Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}

	//�I���̐F�h�炬����
	const float step = 0.03f;
	m_Alpha += m_UpDown ? step : -step;
	if (m_Alpha > 1.0f || m_Alpha < 0.5f) { m_UpDown = !m_UpDown; }

	//�V���b�v�\��
	if (m_Select == 2) { m_Alpha8 += 0.2f; }
	else { m_Alpha8 = 0.0f; }
	if (m_Alpha8 >= 1.0f) { m_Alpha8 = 1.0f; }

	//�w���v�\��
	if (m_Select == 3) { m_Alpha2 += 0.2f; }
	else { m_Alpha2 = 0.0f; }
	if (m_Alpha2 >= 1.0f) { m_Alpha2 = 1.0f; }
	m_SpriteTarget->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
	m_SpriteHelp1Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha2));
	m_SpriteHelp2Text->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha2));
	m_SpritePressStart->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha7));
	m_SpriteShopText->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha8));
	m_SpriteDebug->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_debugMode));

}

void TitleLogo::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �g�p����V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);	//�T�C�Y
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z); //��]
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);		//�ړ�
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	//���N���X���\�b�h�̌Ăяo��
	GameObject::Draw();
}