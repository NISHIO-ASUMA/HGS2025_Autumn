//=====================================
//
// テクスチャ処理 [ texture.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TEXTURE_H_ // このマクロ定義がされてなかったら
#define _TEXTURE_H_ // 2重インクルード防止のマクロ定義

//*****************************
// テクスチャ管理クラスを定義
//*****************************
class CTexture
{
public:
	// コンストラクタ・デストラクタ
	CTexture();
	~CTexture();

	// メンバ関数
	HRESULT Load(void);
	void UnLoad(void);
	int Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

private:

	static inline constexpr int NUM_TEXTURE = 128;	// 最大テクスチャ数

	// テクスチャ読み込み配列を作成
	const char* TexName[NUM_TEXTURE] =
	{
		"data\\TEXTURE\\number.png",
		"data\\TEXTURE\\shadow000.jpg",
		"data\\TEXTURE\\effect000.jpg",
		"data\\TEXTURE\\pausemenu_continue.png",
		"data\\TEXTURE\\pausemenu_retry.png",
		"data\\TEXTURE\\pausemenu_quit.png",
		"data\\TEXTURE\\PauseBack.png",
		"data\\TEXTURE\\title.jpg",
		"data\\TEXTURE\\tutorial.jpg",
		"data\\TEXTURE\\game.jpg",
		"data\\TEXTURE\\result.jpg",
		"data\\TEXTURE\\ranking.jpg",
	};

	LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXTURE];	// テクスチャポインタ
	static int m_nNumAll;							// テクスチャカウント変数
};

#endif