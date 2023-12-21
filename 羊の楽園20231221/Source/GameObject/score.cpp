#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\score.h"
#include "..\App\sprite.h"

#define TEXNUM_X 5
#define TEXNUM_Y 5

void Score::Init()
{
	VERTEX_3D vertex[4];

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\score2.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_CountDay = 1;

}

void Score::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Score::Update()
{
	GameObject::Update();

	if (m_Count > 99) { m_Count = 99; }
	if (m_CountCoin > 999) { m_CountCoin = 999; }
	if (m_FullCount < m_Count) { m_FullCount = m_Count; }
}

void Score::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	//���N���X���\�b�h�̌Ăяo��
	GameObject::Draw();

	//�r��
	int count = m_Count;
	for (int i = 0; i < 3; i++) {
		//���_���W�Z�o
		float vx = 70 - i * 20.0f;
		float vy = 30.0f;
		float height = 32.0f;
		float width = 32.0f;

		//�����̒l�����炷
		int number = 0;
		if (i == 2) {
			number = 11;
			vx -= 13;
		}
		else {
			number = count % 10;
			count /= 10;
		}
		DrawData(vx, vy, width, height, number);
	}

	//�R�C��
	int countCoin = m_CountCoin;
	for (int c = 0; c < 4; c++) {
		//���_���W�Z�o
		float vx = 70 + 20 - c * 20.0f;
		float vy = 70.0f;
		float height = 32.0f;
		float width = 32.0f;

		int number = 0;
		//�����̒l�����炷
		if (c == 3) {
			number = 10;
			vx -= 13;
		}
		else {
			number = countCoin % 10;
			countCoin /= 10;
		}
		DrawData(vx, vy, width, height, number);
	}

	//�̗�
	int countLife = m_Life;
	int countFullLife = m_FullLife;
	for (int c = 0; c < 3; c++) {
		//���_���W�Z�o
		float vx = 160 - c * 13.0f;
		float vy = 3.0f;
		float height = 20.0f;
		float width = 20.0f;

		int number = 0;
		//�����̒l�����炷
		if (c == 0) {
			number = countFullLife % 10;
			countFullLife /= 10;
		}
		else if (c == 1) {
			number = 21;
		}
		else if (c == 2) {

			number = countLife % 10;
			countLife /= 10;
		}
		DrawData(vx, vy, width, height, number);
	}

	//����
	int countDay = m_CountDay;
	int cc = 2;
	if (countDay >= 10)//10���ȍ~�Ɍ���������
	{
		cc = 3;
	}
	for (int c = 0; c < cc; c++) {
		//���_���W�Z�o
		float vx = 870 - c * 40.0f;
		float vy = 10.0f;
		float height = 62.0f;
		float width = 62.0f;

		int number = 0;
		//�����̒l�����炷
		if (c == 0) {
			number = 12;
		}
		else {
			number = countDay % 10;
			countDay /= 10;
		}
		DrawData(vx, vy, width, height, number);
	}

	//�R���{
	int countCombo = m_CountCombo;
	if (m_CountCombo > 0) {//�R���{�񐔂���葽����Ε\��
		for (int c = 0; c < 3; c++) {
			//���_���W�Z�o
			float vx = 750 - c * 27.0f;
			float vy = 120.0f;
			float height = 50.0f;
			float width = 50.0f;
			int number = 0;
			//�����̒l�����炷
			if (c == 2) {
				number = 15;
				vx -= 15;
			}
			else {
				number = countCombo % 10;
				countCombo /= 10;
			}
			DrawData(vx, vy, width, height, number);
		}
	}

	//�ׂ��ȃA�C�R����
	for (int c = 0; c < 1; c++) {
		//���_���W�Z�o
		float vx = 863 - c * 40.0f;
		float vy = 75.0f;
		float height = 45.0f;
		float width = 45.0f;
		int number = 0;
		//�����̒l�����炷
		if (c == 0) {//���ԑ�
			if (m_TimeZone) {
				number = 17;
			}
			else if (!m_TimeZone) {
				number = 16;
			}
		}
		DrawData(vx, vy, width, height, number);
	}
}

void Score::DrawData(float vx, float vy, float width, float height, int num)
{

	//�e�N�X�`�����W�Z�o//����	  //�c��
	float x = num % TEXNUM_X * (1.0f / TEXNUM_Y);
	float y = num / TEXNUM_X * (1.0f / TEXNUM_X);

	//���_�f�[�^��������
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//3D���ƍ�������
	vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(x, y);

	//�E��
	vertex[1].Position = D3DXVECTOR3(vx + width, vy, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y);

	//���O
	vertex[2].Position = D3DXVECTOR3(vx, vy + height, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / TEXNUM_Y));

	//�E�O
	vertex[3].Position = D3DXVECTOR3(vx + width, vy + height, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y + (1.0f / TEXNUM_Y));

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw(4, 0);
}
