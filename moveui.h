//==============================================================
//
// 動くUI処理 [ moveui.h ]
// Author: Asuma Nishio
//
//==============================================================

#ifndef _MOVEUI_H_ // このマクロ定義がされてなかったら
#define _MOVEUI_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object2D.h"

//********************************************************
// 動くUIクラスを定義
//********************************************************
class CMoveUi : public CObject2D
{
public:
	//**********************
	// 移動方向種類
	//**********************
	enum MOVETYPE
	{
		MOVETYPE_NONE,
		MOVETYPE_RIGHT,
		MOVETYPE_LEFT,
		MOVETYPE_CENTER,
		MOVETYPE_DOWN,
		MOVETYPE_MAX
	};

	//**********************
	// 拡大方向種類
	//**********************
	enum SCALETYPE
	{
		SCALETYPE_NONE,
		SCALETYPE_CENTER,
		SCALETYPE_LEFTDOWN,
		SCALETYPE_MAX
	};

	// コンストラクタ・デストラクタ
	CMoveUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CMoveUi();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CMoveUi* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, const char* pTexName, MOVETYPE movetype, SCALETYPE sceletype);

private:
	int m_nIdxTexture;	// テクスチャインデックス
	MOVETYPE m_nMoveType;	// 移動種類
	SCALETYPE m_nScaleType;	// 拡大種類
};
#endif