#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\App\sprite.h"
#include "..\GameObject\titleScore.h"


#define TEXNUM_X 5
#define TEXNUM_Y 5

void TitleScore::Init()
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

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\seepList.png",
		NULL,
		NULL,
		&m_Texture2,
		NULL);
	assert(m_Texture2);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
	
}

void TitleScore::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}


void TitleScore::Update()
{
	GameObject::Update();
	if (m_AlphaMove) {
		m_Alpha = 1.0f;
		if (m_Alpha >= 1.0f) { m_Alpha = 1.0f; }
	}
	else {
		m_Alpha = 0.0f;
	}
}

void TitleScore::Draw()
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

	//�R�C��
	int countCoin = m_CountCoin;
	for (int c = 0; c < 6; c++) {
		//���_���W�Z�o
		float vx = 530 - c * 22.0f;
		float vy = 400.0f;
		float height = 30.0f;
		float width = 30.0f;

		int number = 0;

		//�����̒l�����炷
		if (c == 5) {
			number = 10;
			vx -= 5;
		}
		else {
			number = countCoin % 10;
			countCoin /= 10;
		}

		//�e�N�X�`�����W�Z�o//����	  //�c��
		float x = number % TEXNUM_X * (1.0f / TEXNUM_Y);
		float y = number / TEXNUM_X * (1.0f / TEXNUM_X);

		//���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//3D���ƍ�������
		vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		//�E��
		vertex[1].Position = D3DXVECTOR3(vx + width, vy, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y);

		//���O
		vertex[2].Position = D3DXVECTOR3(vx, vy + height, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / TEXNUM_Y));

		//�E�O
		vertex[3].Position = D3DXVECTOR3(vx + width, vy + height, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
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

	//�V���b�v�r
	int pcolor = m_PlColor;
	int pclown = m_PlClown;
	for (int c = 0; c < 2; c++) {
		//���_���W�Z�o
		float vx = 420;
		float vy = 180.0f;
		float height = 130.0f;
		float width = 130.0f;

		int number = 0;
		//���炷
		if (c == 1) { number = pcolor+10; }
		if (c == 0) { number = pclown; }
		

		//�e�N�X�`�����W�Z�o//����	  //�c��
		float x = number % TEXNUM_X * (1.0f / TEXNUM_Y);
		float y = number / TEXNUM_X * (1.0f / TEXNUM_X);

		//���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//3D���ƍ�������
		vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		//�E��
		vertex[1].Position = D3DXVECTOR3(vx + width, vy, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y);

		//���O
		vertex[2].Position = D3DXVECTOR3(vx, vy + height, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / TEXNUM_Y));

		//�E�O
		vertex[3].Position = D3DXVECTOR3(vx + width, vy + height, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, m_Alpha);
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
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture2);

		// �v���~�e�B�u�g�|���W�ݒ�
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// �|���S���`��
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}
