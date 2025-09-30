//==============================================
//
// リザルトシーン管理処理 [ resultmanager.h ]
// Author: Asuma Nishio
// 
//==============================================

#ifndef _RESULTMANAGER_H_ // このマクロ定義がされてなかったら
#define _RESULTMANAGER_H_ // 2重インクルード防止のマクロ定義

//**************************
// 前方宣言
//**************************
class CResultScore;

//**************************
// リザルト管理クラスを定義
//**************************
class CResultManager
{
public:
	// コンストラクタ・デストラクタ
	CResultManager();
	~CResultManager();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Load(void);
	void MathScore(void);

private:
	static constexpr int MAX_VALUESCORE = 500000;
	static constexpr int ADDSCORETIME = 60;

	bool m_isKeyDown; // キー入力フラグ
	int m_nGameScore; // スコアを格納
	int m_nLastTime; // 時間を格納
	int m_nBulletScore; // 弾
	int m_nAllScore;

	CResultScore* m_pResultScore[4];	// スコアポインタ
};

#endif

