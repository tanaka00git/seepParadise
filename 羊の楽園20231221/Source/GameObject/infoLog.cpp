#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\infoLog.h"
#include "..\App\sprite.h"

void InfoLog::Load()
{
}

void InfoLog::Unload()
{
}

void InfoLog::Init()
{
	//�����ɃV�F�[�_�[�t�@�C���̃��[�h��ǉ�
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
	
	m_Alpha = 0.0f;
}

void InfoLog::Uninit()
{
	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void InfoLog::Update()
{
	GameObject::Update();

	//��{�C�x���g
	if (m_MoveNum == 1) {
		if (!m_LogEnd) {
			m_Alpha += 0.05f;
			if (m_Alpha >= 1.0f) {
				m_Alpha = 1.0f;
				m_DeleteTime ++;
				if (m_DeleteTime > 120) { m_LogEnd = true; }//�\������Ă��Ԃ̈ێ�
			}
		}
		else if (m_LogEnd) {//�I�������ɂȂ�����
			m_Position.y --;
			m_Alpha -= 0.02f;
			if (m_Alpha <= 0.0f) {
				m_Alpha = 0.0f;
				SetDestroy();
			}
		}
	}
	//�R�C�����o
	else if (m_MoveNum == 2) {
		m_Position.y -= 0.8f;
		m_Alpha -= 0.02f;
		if (m_Alpha <= 0.0f) {
			m_Alpha = 0.0f;
			SetDestroy();
		}
	}
	//�j�R�j�R�R�����g���o
	else if (m_MoveNum == 3) {
		m_Alpha = 1.0f;
		m_Position.x -= 2.0f;
		if (m_Position.x <= -250.0f) {
			SetDestroy();
		}
	}
	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
	m_Sprite->SetPosition(m_Position);
}


void InfoLog::Draw()
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

void InfoLog::SetNum(int num, int moveNum, D3DXVECTOR3 position)
{
	m_LogNum = num;
	m_MoveNum = moveNum;

	m_Position = position;

	if (m_LogNum == 1) {//��
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog1.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 2) {//��
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog2.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 4) {//�_���[�W
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog4.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 5) {//��
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog5.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 6) {//����
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog6.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 7) {//�C�x���g�u�r�̌Q��v
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog7.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 8) {//�C�x���g�u�T�������v
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog8.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 9) {//�C�x���g�I��
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog9.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 10) {//���x�㏸
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog10.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 11) {//�̗͏㏸
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog11.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 12) {//����㏸
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog12.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 13) {//�ЊQ�T�o��
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog13.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 44) {//-100�~
	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog44.png");
	m_Alpha = 1.0f;
	}

	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
	m_Sprite->SetPosition(m_Position);

}
