//==========================================
//
// ランキングスコア処理 [ rankingscore.h ]
// Author: Asuma Nishio
//
//==========================================

#ifndef _RANKINGSCORE_H_ // このマクロ定義がされてなかったら
#define _RANKINGSCORE_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"

//**********************
// 前方宣言
//**********************
class CNumber;

//******************************
// ランキングスコアクラス定義
//******************************
class CRankingScore : public CObject
{
public:
	// コンストラクタ・デストラクタ
	CRankingScore(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CRankingScore();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Load(void);

	// セッター
	void SetTexture(void);

	// 静的メンバ関数
	static CRankingScore* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static void SetRankInScoreIdx(int nIdxScore) { m_nNewRankingScore = nIdxScore; }

private:
	// 定数
	static constexpr int RANKSCOREDIGIT = 8;	// 桁数
	static constexpr int RANKING_MAX = 5;		// ランキング数

	// 静的変数
	static int m_nNewRankingScore;	// 順位に入っているかどうか

	int m_nIdxTex;					// テクスチャインデックス
	int m_aRankScore[RANKING_MAX];	// スコア配列
	CNumber* m_apNumber[RANKING_MAX][RANKSCOREDIGIT]; // 各スコアと桁数

	D3DXVECTOR3 m_pos;		// 座標
	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ
};

#endif