#pragma once
#include "..\GameObject\gameObject.h"

class TitleLogo : public GameObject
{
private:

	ID3D11VertexShader* m_VertexShader{};
	ID3D11PixelShader* m_PixelShader{};
	ID3D11InputLayout* m_VertexLayout{};

	class Sprite* m_SpriteLogo{};
	class Sprite* m_SpriteStart{};
	class Sprite* m_SpriteShop{};
	class Sprite* m_SpriteHelp{};
	class Sprite* m_SpriteTarget{};
	class Sprite* m_SpriteHelp1Text{};
	class Sprite* m_SpriteHelp2Text{};
	class Sprite* m_SpritePressStart{};
	class Sprite* m_SpriteShopText{};
	class Sprite* m_SpriteDebug{};


	float m_Alpha = 1.0f;
	float m_Alpha2 = 0.0f;
	float m_Alpha7 = 1.0f;
	float m_Alpha8 = 0.0f;
	bool  m_debugMode = false;


	int m_Scene = 0;		//現在のシーンは何番目
	int m_Select =	1;		//選択肢の番号
	bool m_UpDown = false;	//演出用
	float m_Time = 0.0f;	//ボタン
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


	void SetScene(int i) {m_Scene = i;}
	void SetSelect(int i) { m_Select = i; }
	int  GetScene() { return m_Scene; }
	int  GetSelect1() { return m_Select; }
	void SetDebug(bool debug) { m_debugMode = debug; }

};