//=====================================
//
// 攻撃サイン処理 [ signal.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _SIGNAL_H_ // このマクロ定義がされてなかったら
#define _SIGNAL_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object2D.h"

//**********************
// 前方宣言
//**********************
class CObject;

//*************************
// 攻撃サインクラスを定義
//*************************
class CSignal : public CObject2D
{
public:
	// コンストラクタ・デストラクタ
	CSignal(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CSignal();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CSignal* Create(void);

private:
	int m_nLife;	// 寿命
};

#endif