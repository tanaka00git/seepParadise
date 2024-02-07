#include "..\App\main.h"
#include "..\App\manager.h"
#include "..\App\renderer.h"
#include "..\Scene\scene.h"
#include "..\GameObject\camera.h"
#include "..\GameObject\hpBarS.h"

ID3D11ShaderResourceView*HpBarS::m_Texture{};

void HpBarS::Load()
{
	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset\\texture\\fadeOver.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);
}

void HpBarS::Unload()
{
	m_Texture->Release();
}

void HpBarS::Init()
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

	//�ύX�AunlitTextureVS��2D�p�A3D��vertexLightingVS
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
}

void HpBarS::Uninit()
{
	m_VertexBuffer->Release();

	//�����ɃV�F�[�_�[�I�u�W�F�N�g�̉����ǉ�
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void HpBarS::Update()
{
}

void HpBarS::Draw()
{
	//m_Dwaw��false�Ȃ�\�����Ȃ�
	if (!m_Draw) { return; }

	Scene* scene = Manager::GetScene();

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_��ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//�J�����̃r���[�}�g���N�X�̎擾
	Camera* camera = scene->GetGameObject<Camera>();
	D3DXMATRIX view = camera->GetViewMatrix();

	//�r���[�̋t�s��
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);//�t�s��
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	float weight = 0.4f;
	float height = 0.05f;
	float weight2 = weight;
	float vz = 0.0f;
	float vy = -0.7f;

	D3DXVECTOR4 Diffuse = D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f);
	float Conversion = (m_CurrentLife / m_FullLife) * (weight * 2) - weight;
	float Conversion2 = Conversion;

	for (int i = 0; i < 3 + m_ChargeOn * 2; i++) {

		if (i == 1) {//��

			float distance = Conversion - m_OldConversion;
			float speed = 0.1f; // �ړ����x�𒲐�����l
			float velocity = distance * speed;
			float newConversion = m_OldConversion + velocity;
			Conversion = newConversion;
			weight2 = Conversion;
			m_OldConversion = Conversion;

			Diffuse = D3DXVECTOR4(1.0f, 0.2f, 0.2f, 1.0f);
		}
		else  if (i == 2) {//��

			float distance = Conversion2 - m_OldConversion2;
			float speed = 0.2f; // �ړ����x�𒲐�����l
			float velocity = distance * speed;
			float newConversion = m_OldConversion2 + velocity;
			Conversion2 = newConversion;
			weight2 = Conversion2;
			m_OldConversion2 = Conversion2;

			Diffuse = D3DXVECTOR4(0.4f, 0.8f, 0.4f, 1.0f);
		}
		else  if (i == 3) {//��2
			Diffuse = D3DXVECTOR4(0.4f, 0.4f, 0.4f, 1.0f);
			height = 0.015f;
			weight2 = weight;
			vy -= 0.045f;
		}
		else  if (i == 4) {//��
			if (m_CurrentCharge >= m_FullCharge) {
				m_CurrentCharge = m_FullCharge;
				Diffuse = D3DXVECTOR4(1.0f, 0.5f, 0.5f, 1.0f);
			}
			else if (m_DashOn) { Diffuse = D3DXVECTOR4(1.0f, 0.5f, 0.5f, 1.0f); }//����o�[
			else {Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f); }//���o�[

			weight2 = (m_CurrentCharge / m_FullCharge) * (weight * 2) - weight;
		}

		//���_�f�[�^��������
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;
		//3D���ƍ�������
		vertex[0].Position = D3DXVECTOR3(-weight, height+ vy, vz);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = Diffuse;
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		//�E��
		vertex[1].Position = D3DXVECTOR3(weight2, height + vy, vz);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = Diffuse;
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		//���O
		vertex[2].Position = D3DXVECTOR3(-weight, -height + vy, vz);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = Diffuse;
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		//�E�O
		vertex[3].Position = D3DXVECTOR3(weight2, -height + vy, vz);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = Diffuse;
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

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

}