//======================================
//
//  リセットゲージ処理[gauge_reset.h]
//  Author : Kaiti Aoto
//
//======================================

// 二重インクルード防止
#ifndef _GAUGE_CHARGE_H_
#define _GAUGE_CHARGE_H_

// インクルード
#include "main.h"
#include "gauge.h"
#include "object2D.h"

// リセットゲージクラス
class CChargeGauge :CObject
{
public:

	// メンバ関数
	CChargeGauge(int nPriority = 6);
	~CChargeGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CChargeGauge* Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);

private:
	// メンバ変数
	D3DXVECTOR3 m_pos;		// 位置
	D3DXCOLOR m_col;		// 色
	float m_Base;			// 基準
	float m_fHeight;		// 高さ	
	CGauge* m_pGauge;		// ゲージへのポインタ
	bool m_bUse;			// 使用しているか
	int m_nCntReset;		// リセット完了までの時間カウント	
	CObject2D* m_pObj2D;	// 2Dオブジェクトへのポインタ
};
#endif
