//==========================================
//
// ランキング管理処理 [ rankingmanager.h ]
// Author: Asuma Nishio
//
//==========================================

#ifndef _RANKINGMANAGER_H_ // このマクロ定義がされてなかったら
#define _RANKINGMANAGER_H_ // 2重インクルード防止のマクロ定義

//******************************
// 前方宣言
//******************************
class CResultScore;

//******************************
// ランキング管理クラスを定義
//******************************
class CRankingManager
{
public:
	// コンストラクタ・デストラクタ
	CRankingManager();
	~CRankingManager();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	// クラスポインタ

};

#endif