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
	//ここにシェーダーファイルのロードを追加
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");
	
	m_Alpha = 0.0f;
}

void InfoLog::Uninit()
{
	//ここにシェーダーオブジェクトの解放を追加
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void InfoLog::Update()
{
	GameObject::Update();

	//基本イベント
	if (m_MoveNum == 1) {
		if (!m_LogEnd) {
			m_Alpha += 0.05f;
			if (m_Alpha >= 1.0f) {
				m_Alpha = 1.0f;
				m_DeleteTime ++;
				if (m_DeleteTime > 120) { m_LogEnd = true; }//表示されてる状態の維持
			}
		}
		else if (m_LogEnd) {//終了条件になったら
			m_Position.y --;
			m_Alpha -= 0.02f;
			if (m_Alpha <= 0.0f) {
				m_Alpha = 0.0f;
				SetDestroy();
			}
		}
	}
	//コイン演出
	else if (m_MoveNum == 2) {
		m_Position.y -= 0.8f;
		m_Alpha -= 0.02f;
		if (m_Alpha <= 0.0f) {
			m_Alpha = 0.0f;
			SetDestroy();
		}
	}
	//ニコニココメント演出
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
	//入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// 使用するシェーダを設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();


	//基底クラスメソッドの呼び出し
	GameObject::Draw();
}

void InfoLog::SetNum(int num, int moveNum, D3DXVECTOR3 position)
{
	m_LogNum = num;
	m_MoveNum = moveNum;

	m_Position = position;

	if (m_LogNum == 1) {//朝
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog1.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 2) {//夜
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog2.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 4) {//ダメージ
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog4.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 5) {//回復
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog5.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 6) {//日数
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog6.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 7) {//イベント「羊の群れ」
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog7.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 8) {//イベント「狼が来た」
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 160, "asset\\texture\\timeLog8.png");
		m_Alpha = 0.0f;
	}
	else if (m_LogNum == 9) {//イベント終了
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog9.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 10) {//速度上昇
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog10.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 11) {//体力上昇
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog11.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 12) {//視野上昇
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog12.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 13) {//災害狼出現
		m_Sprite = AddComponent<Sprite>();
		m_Sprite->Init(0.0f, 0.0f, 300, 60, "asset\\texture\\timeLog13.png");
		m_Alpha = 1.0f;
	}
	else if (m_LogNum == 44) {//-100円
	m_Sprite = AddComponent<Sprite>();
	m_Sprite->Init(0.0f, 0.0f, 80, 80, "asset\\texture\\timeLog44.png");
	m_Alpha = 1.0f;
	}

	m_Sprite->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_Alpha));
	m_Sprite->SetPosition(m_Position);

}
