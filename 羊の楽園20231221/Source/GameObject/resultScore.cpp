#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\resultScore.h"
#include "..\App\sprite.h"

const int TEXNUM_X = 5;
const int TEXNUM_Y = 5;

void ResultScore::Init()
{
	VERTEX_3D vertex[4];

	// 頂点バッファ生成
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


	// テクスチャ読み込み
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

void ResultScore::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();

}


void ResultScore::Update()
{
	GameObject::Update();
}


void ResultScore::Draw()
{

	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//基底クラスメソッドの呼び出し
	//GameObject::Draw();

	//羊数
	int count = m_CountSheep;
	
	for (int i = 0; i < 3; i++) {
		//頂点座標算出
		float vx = 425 - i * 33.0f;
		float vy = 105.0f;
		float height = 50.0f;
		float width = 50.0f;

		//桁内の値をずらす
		int number = 0;		
		number = count % 10;
		count /= 10;

		//テクスチャ座標算出//横横	  //縦横
		float x = number % TEXNUM_X * (1.0f / TEXNUM_Y);
		float y = number / TEXNUM_X * (1.0f / TEXNUM_X);

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//3Dだと左奥から
		vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		//右奥
		vertex[1].Position = D3DXVECTOR3(vx + width, vy, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		//vertex[1].TexCoord = D3DXVECTOR2(x + 0.2, y);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y);

		//左前
		vertex[2].Position = D3DXVECTOR3(vx, vy + height, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		//vertex[2].TexCoord = D3DXVECTOR2(x, y + 0.2f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / TEXNUM_Y));

		//右前
		vertex[3].Position = D3DXVECTOR3(vx + width, vy + height, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		//vertex[3].TexCoord = D3DXVECTOR2(x + 0.2f, y + 0.2f);
		vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y + (1.0f / TEXNUM_Y));

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		// テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}

	//コイン
	int countCoin = m_CountCoin;
	for (int c = 0; c < 3; c++) {
		//頂点座標算出
		float vx = 425 - c * 33.0f;
		float vy = 198.0f;
		float height = 50.0f;
		float width = 50.0f;

		int number = 0;
		//桁内の値をずらす
		number = countCoin % 10;
		countCoin /= 10;

		//テクスチャ座標算出//横横	  //縦横
		float x = number % TEXNUM_X * (1.0f / TEXNUM_Y);
		float y = number / TEXNUM_X * (1.0f / TEXNUM_X);

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//3Dだと左奥から
		vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		//右奥
		vertex[1].Position = D3DXVECTOR3(vx + width, vy, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y);

		//左前
		vertex[2].Position = D3DXVECTOR3(vx, vy + height, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / TEXNUM_Y));

		//右前
		vertex[3].Position = D3DXVECTOR3(vx + width, vy + height, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y + (1.0f / TEXNUM_Y));

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		// テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}

	//日数
	int countDay = m_CountDay;
	int cc = 2;
	if (countDay >= 10)//10日以降に桁が増える
	{
		cc = 3;
	}
	for (int c = 0; c < cc; c++) {
		//頂点座標算出
		float vx = 420 - c * 40.0f;
		float vy = 280.0f;
		float height = 65.0f;
		float width = 62.0f;

		int number = 0;
		//桁内の値をずらす
		number = countDay % 10;
		countDay /= 10;
		
		//テクスチャ座標算出//横横	  //縦横
		float x = number % TEXNUM_X * (1.0f / TEXNUM_Y);
		float y = number / TEXNUM_X * (1.0f / TEXNUM_X);

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//3Dだと左奥から
		vertex[0].Position = D3DXVECTOR3(vx, vy, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(x, y);

		//右奥
		vertex[1].Position = D3DXVECTOR3(vx + width, vy, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y);

		//左前
		vertex[2].Position = D3DXVECTOR3(vx, vy + height, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(x, y + (1.0f / TEXNUM_Y));

		//右前
		vertex[3].Position = D3DXVECTOR3(vx + width, vy + height, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(x + (1.0f / TEXNUM_X), y + (1.0f / TEXNUM_Y));

		Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

		// マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		material.TextureEnable = true;
		Renderer::SetMaterial(material);

		// テクスチャ設定
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		// プリミティブトポロジ設定
		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// ポリゴン描画
		Renderer::GetDeviceContext()->Draw(4, 0);
	}
}