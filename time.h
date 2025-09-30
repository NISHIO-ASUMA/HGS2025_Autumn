//=====================================
//
// タイム処理 [ time.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TIME_H_ // このマクロ定義がされてなかったら
#define _TIME_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"

//**********************
// 前方宣言
//**********************
class CNumber;

//**********************************
// タイムクラスを定義
//**********************************
class CTime : public CObject
{
public:
	// コンストラクタ・デストラクタ
	CTime(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CTime();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Save(void);

	// ゲッター
	int GetAllTime(void) { return m_nAllTime; }
	int GetcurrentTime(void) { return m_nSecond; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	// 静的メンバ関数
	static CTime* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	// メンバ関数
	void Second(void);
	void Minute(void);

	static constexpr int DIGIT_TIME = 2;		// 桁数
	static constexpr int NUMTIME = 60;			// 最大タイマー
	static constexpr int CARVETIME = 60;		// カウント上限
	static constexpr int DIVIDE = 10;			// 桁分割の値
	static constexpr int REDTIMEZONE = 10;		// 赤く点滅させる時間
	static inline constexpr float VALUE_WIDTH = 150.0f; // 横幅の間隔

	int m_nAllTime;		// 全体の時間
	int m_nSecond;		// 現在の時間 ( 秒 )
	int m_nMinute;		// 現在の時間 (分 )
	int m_nDecTime;		// 減った時間量
	int m_nCount;		// 時間カウンター
	float m_fHeight, m_fWidth;	// 横幅,高さ

	CNumber* m_pNumberMinute[DIGIT_TIME];	// 分
	CNumber* m_pNumberSecond[DIGIT_TIME];	// 秒
	D3DXVECTOR3 m_pos;		// 座標
};
#endif