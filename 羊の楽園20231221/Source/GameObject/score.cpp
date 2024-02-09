#include "..\App\main.h"
#include "..\App\renderer.h"
#include "..\GameObject\score.h"
#include "..\App\sprite.h"

#define TEXNUM_X 5
#define TEXNUM_Y 5
#define PLUS_SCALE_VALUE 5.0f
#define PLUS_SCALE_DOWN_VALUE 0.5f

void Score::Init()
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

void Score::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Score::Update()
{
	GameObject::Update();

	if (m_CountSeep > 100) { m_CountSeep = m_ClearCount; }
	if (m_CountCoin > 999) { m_CountCoin = 999; }
	if (m_FullCount < m_CountSeep) { m_FullCount = m_CountSeep; }


	//羊の増減でスコアのスケールが変わる処理
	if (m_CountSeep != m_OldCountSeep)
	{
		if (m_CountSeep > m_OldCountSeep) { m_CountPlusScale = PLUS_SCALE_VALUE; }
		else { m_CountMinusScale = PLUS_SCALE_VALUE; }
	}
	m_OldCountSeep = m_CountSeep;

	m_CountPlusScale -= PLUS_SCALE_DOWN_VALUE;
	m_CountMinusScale -= PLUS_SCALE_DOWN_VALUE;
	if (m_CountPlusScale < 0.0f) { m_CountPlusScale = 0.0f; }
	if (m_CountMinusScale < 0.0f) { m_CountMinusScale = 0.0f; }

}

void Score::Draw()
{
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	//基底クラスメソッドの呼び出し
	GameObject::Draw();

	//それぞれの描画
	DrawCountSeep();
	DrawCountClearSeep();
	DrawCountCombo();
	DrawTimeZone();
	DrawLife();
}

void Score::DrawCountSeep()
{
	int count = m_CountSeep;
	for (int i = 0; i < 4; i++)
	{
		//頂点座標算出
		float vx = 85 - i * 20.0f;
		float vy = 30.0f;
		float height = 32.0f + m_CountPlusScale - m_CountMinusScale;
		float width = 32.0f + m_CountPlusScale - m_CountMinusScale;

		//桁内の値をずらす
		int number = 0;
		if (i == 3) {
			number = 11;
			vx -= 8;
		}
		else {
			number = count % 10;
			count /= 10;
		}
		DrawData(vx, vy, width, height, number);
	}
}

void Score::DrawCountClearSeep()
{
	int count = m_ClearCount;
	for (int i = 0; i < 4; i++)
	{
		//頂点座標算出
		float vx = 150 - i * 15.0f;
		float vy = 38.0f;
		float height = 24.0f;
		float width = 24.0f;

		//桁内の値をずらす
		int number = 0;
		if (i == 3) {
			number = 21;
		}
		else {
			number = count % 10;
			count /= 10;
		}
		DrawData(vx, vy, width, height, number);
	}
}

void Score::DrawCountCombo()
{
	int countCombo = m_CountCombo;
	if (m_CountCombo > 0) {//コンボ回数がより多ければ表示
		for (int c = 0; c < 3; c++) {
			//頂点座標算出
			float vx = 750 - c * 27.0f;
			float vy = 120.0f;
			float height = 50.0f;
			float width = 50.0f;
			int number = 0;
			//桁内の値をずらす
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
}

void Score::DrawTimeZone()
{
	for (int c = 0; c < 1; c++) {
		//頂点座標算出
		float vx = 865;
		float vy = 0.0f;
		float height = 60.0f;
		float width = 60.0f;
		int number = 0;
		//桁内の値をずらす
		if (c == 0) {//時間帯
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

void Score::DrawLife()
{
	//体力
	int countLife = m_Life;
	int countFullLife = m_FullLife;
	for (int c = 0; c < 3; c++) {
		//頂点座標算出
		float vx = 160 - c * 13.0f;
		float vy = 3.0f;
		float height = 20.0f;
		float width = 20.0f;

		int number = 0;
		//桁内の値をずらす
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
}

void Score::DrawData(float vx, float vy, float width, float height, int num)
{

	//テクスチャ座標算出//横横	  //縦横
	float x = num % TEXNUM_X * (1.0f / TEXNUM_Y);
	float y = num / TEXNUM_X * (1.0f / TEXNUM_X);

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
