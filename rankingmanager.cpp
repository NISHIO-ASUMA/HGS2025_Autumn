//===========================================
//
// ランキング管理処理 [ rankingmanager.cpp ]
// Author: Asuma Nishio
//
//===========================================

//******************************
// インクルードファイル
//******************************
#include "rankingmanager.h"
#include "rankingscore.h"
#include "ui.h"

//================================
// コンストラクタ
//================================
CRankingManager::CRankingManager()
{
	// 値のクリア

}
//================================
// デストラクタ
//================================
CRankingManager::~CRankingManager()
{
	// 無し
}
//================================
// 初期化処理
//================================
HRESULT CRankingManager::Init(void)
{
	// Ui背景生成
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "RankBack.jpg", false);

	// ランキングスコア生成
	CRankingScore::Create(D3DXVECTOR3(900.0f, 200.0f, 0.0f), 250.0f, 40.0f);

	// 初期化結果を返す
	return S_OK;
}
//================================
// 終了処理
//================================
void CRankingManager::Uninit(void)
{
	// 無し
}
//================================
// 更新処理
//================================
void CRankingManager::Update(void)
{
	// 無し
}