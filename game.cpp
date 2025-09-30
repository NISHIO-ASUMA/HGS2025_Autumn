//=====================================
//
// メインゲーム処理 [ game.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// インクルードファイル宣言
//****************************
#include "game.h"
#include "manager.h"
#include "debugproc.h"
#include "title.h"
#include "result.h"
#include "ui.h"
#include "player.h"
#include "score.h"

//**************************
// 静的メンバ変数宣言
//**************************
CPauseManager* CGame::m_pPausemanager = nullptr; // ポーズマネージャーのポインタ
CBlockManager* CGame::m_pBlockManager = nullptr; // ブロックマネージャーのポインタ
CTime* CGame::m_pTime = nullptr;				 // タイムクラスのポインタ

//==================================
// コンストラクタ
//==================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// 値のクリア
	m_nGametype = GAMESTATE_NONE;
	m_nStateCount = NULL;
}
//==================================
// デストラクタ
//==================================
CGame::~CGame()
{
	// 無し
}
//==================================
// 初期化処理
//==================================
HRESULT CGame::Init(void)
{
	// ポーズマネージャー生成
	m_pPausemanager = new CPauseManager;

	// nullだったら
	if (m_pPausemanager == nullptr)
	{
		return E_FAIL;
	}

	// ポーズマネージャー初期化処理
	m_pPausemanager->Init();

	// ブロックマネージャーの生成
	m_pBlockManager = new CBlockManager;

	// ブロックマネージャーの初期化
	m_pBlockManager->Init();

	// JSONの読み込み
	m_pBlockManager->LoadFromJson("data/test_01.json");

	// 通常進行状態
	m_nGametype = GAMESTATE_NORMAL;

	// プレイヤーの生成
	CPlayer* player = CCharacterManager::GetInstance().AddCharacter<CPlayer>();

	// キャラクターの初期化処理
	CCharacterManager::GetInstance().InitAll();

	// タイマー生成
	m_pTime = CTime::Create(D3DXVECTOR3(560.0f, 40.0f, 0.0f), 60.0f, 40.0f);
	
	// コロン生成
	CUi::Create(D3DXVECTOR3(665.0f, 40.0f, 0.0f), 0, 15.0f, 25.0f, "coron.png", false);

	// スコア生成
	CScore::Create(D3DXVECTOR3(1240.0f, 660.0f, 0.0f), 180.0f, 60.0f);

	// 初期化結果を返す
	return S_OK;
}
//==================================
// 終了処理
//==================================
void CGame::Uninit(void)
{
	// キャラクターの終了処理
	CCharacterManager::GetInstance().Uninit();

	// ブロックマネージャーの破棄
	if (m_pBlockManager != nullptr)
	{
		m_pBlockManager->Uninit();

		delete m_pBlockManager;
		m_pBlockManager = nullptr;
	}

	// nullチェック
	if (m_pPausemanager != nullptr)
	{
		// 終了処理
		m_pPausemanager->Uninit();

		// ポインタの破棄
		delete m_pPausemanager;

		// nullptrにする
		m_pPausemanager = nullptr;
	}
}
//==================================
// 更新処理
//==================================
void CGame::Update(void)
{	
	// ポーズのキー入力判定
	m_pPausemanager->SetEnablePause();
	
	// ポーズの更新処理
	m_pPausemanager->Update();
	
	// ブロックマネージャーの更新処理
	m_pBlockManager->Update();

	// フェード取得
	CFade* pFade = CManager::GetFade();

	switch (m_nGametype)
	{
	case GAMESTATE_NORMAL://通常状態
		break;

	case GAMESTATE_END:
		m_nStateCount++;

		if (m_nStateCount >= 60)
		{
			// カウンターを初期化
			m_nStateCount = 0;
			
			// 1秒経過
			m_nGametype = GAMESTATE_NONE;//何もしていない状態

			// フェードが取得できたら
			if (pFade != nullptr)
			{
				// リザルトシーンに遷移
				pFade->SetFade(new CResult());

				// ここで処理を返す
				return;
			}
		}
		break;
	}

	// falseの時に更新
	if (m_pPausemanager->GetPause() == false)
	{
		// タイムが0以下の時
		if (m_pTime->GetAllTime() == NULL)
		{
			m_nGametype = GAMESTATE_END;
		}

#ifdef _DEBUG
		// キー入力
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
		{
			if (pFade != nullptr)
			{
				pFade->SetFade(new CResult());

				return;
			}
		}
#endif // _DEBUG
	}
}
//==================================
// 描画処理
//==================================
void CGame::Draw(void)
{
	// 無し
}
//==================================
// コンストラクタ
//==================================
CGame* CGame::Create(void)
{	
	// インスタンス生成
	CGame* pGame = new CGame;

	// nullptrだったら
	if (pGame == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pGame->Init()))
	{
		// ポインタを返す
		return nullptr;
	}

	// 生成されたポインタを返す
	return pGame;
}