#pragma once

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;
};

// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// ���f���\����
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

class Model
{
private:
	ID3D11Buffer*	m_VertexBuffer;
	ID3D11Buffer*	m_IndexBuffer;

	SUBSET*	m_SubsetArray;
	unsigned int	m_SubsetNum;

	void LoadObj(const char *fileName, MODEL *model);
	void LoadMaterial( const char *FileName, MODEL_MATERIAL **materialArray, unsigned int *materialNum );

public:
	void Draw();
	void DrawColor(D3DXCOLOR color, bool textureEnable);

	void Load( const char *fileName );
	void Unload();
};