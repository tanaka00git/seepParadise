#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\hpBar.h"
#include "..\App\sprite.h"
#include "..\GameObject\player.h"

#define GREEN_LIFE_SPEED 0.2f
#define RED_LIFE_SPEED   0.1f

void HpBar::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Sprite_BlackLife = AddComponent<Sprite>();
	m_Sprite_BlackLife->Init(10.0f, 10.0f, 180, 10, "asset\\texture\\fadeOver.png");
	m_Sprite_BlackLife->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));//���o�[
	m_Sprite_RedLife = AddComponent<Sprite>();
	m_Sprite_RedLife->Init(10.0f, 10.0f, 180, 10, "asset\\texture\\fadeOver.png");
	m_Sprite_RedLife->SetColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));//�ԃo�[
	m_Sprite_GreenLife = AddComponent<Sprite>();
	m_Sprite_GreenLife->Init(10.0f, 10.0f, 180, 10, "asset\\texture\\fadeOver.png");
	m_Sprite_GreenLife->SetColor(D3DXCOLOR(0.4f, 0.8f, 0.4f, 1.0f));//�΃o�[
	m_Sprite_BlackCharge = AddComponent<Sprite>();
	m_Sprite_BlackCharge->Init(10.0f, 20.0f, 180, 5, "asset\\texture\\fadeOver.png");
	m_Sprite_BlackCharge->SetColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));//���o�[�`���[�W
	m_Sprite_WhiteCharge = AddComponent<Sprite>();
	m_Sprite_WhiteCharge->Init(10.0f, 20.0f, 180, 5, "asset\\texture\\fadeOver.png");
	m_Sprite_WhiteCharge->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));//���o�[�`���[�W
}

void HpBar::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void HpBar::Update()
{
	GameObject::Update();

	Scene* scene = Manager::GetScene();
	Player* player = scene->GetGameObject<Player>();

	//�̗͍X�V
	m_FullLife = static_cast<float>(player->GetFullLife());
	m_CurrentLife = static_cast<float>(player->GetLife());
	LifeBarMove();

	//�`���[�W�X�V
	m_CurrentCharge = static_cast<float>(player->GetCharge());
	m_FullCharge = static_cast<float>(player->GetFullCharge());
	ChargeBarMove();
}


void HpBar::Draw()
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

void HpBar::LifeBarMove()
{
	float Conversion = (m_CurrentLife / m_FullLife) * 180;
	float Conversion2 = Conversion;

	//�΃o�[
	float distance = Conversion - m_OldConversion;
	float speed = GREEN_LIFE_SPEED; // �ړ����x�𒲐�����l
	float velocity = distance * speed;
	float newConversion = m_OldConversion + velocity;
	Conversion = newConversion;
	m_Sprite_GreenLife->SetScale(D3DXVECTOR3(Conversion, 10, 0));
	m_OldConversion = Conversion;

	//�ԃo�[
	distance = Conversion2 - m_OldConversion2;
	speed = RED_LIFE_SPEED; // �ړ����x�𒲐�����l
	velocity = distance * speed;
	newConversion = m_OldConversion2 + velocity;
	Conversion2 = newConversion;
	m_Sprite_RedLife->SetScale(D3DXVECTOR3(Conversion2, 10, 0));
	m_OldConversion2 = Conversion2;
}

void HpBar::ChargeBarMove()
{
	//�`���[�W������̐F�ύX
	if (m_CurrentCharge >= m_FullCharge)
	{
		m_CurrentCharge = m_FullCharge;
		m_Sprite_WhiteCharge->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
	}
	else { m_Sprite_WhiteCharge->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); }//���o�[

	float Conversion3 = (m_CurrentCharge / m_FullCharge) * 180;
	m_Sprite_WhiteCharge->SetScale(D3DXVECTOR3(Conversion3, 5, 0));
}

