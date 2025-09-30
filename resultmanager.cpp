//==============================================
//
// リザルトシーン管理処理 [ resultmanager.cpp ]
// Author: Asuma Nishio
// 
//==============================================

//**************************
// インクルードファイル宣言
//**************************
#include "resultmanager.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "title.h"
#include "resultscore.h"
#include "ui.h"
#include "ranking.h"

//=================================
// コンストラクタ
//=================================
CResultManager::CResultManager()
{
	// 値のクリア
	m_isKeyDown = false;
	m_nGameScore = NULL;
	m_nLastTime = NULL;
	m_pResultScore = nullptr;
}
//=================================
// デストラクタ
//=================================
CResultManager::~CResultManager()
{
	// 無し
}
//=================================
// 初期化処理
//=================================
HRESULT CResultManager::Init(void)
{	
	// UI生成
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "resultback.jpg", false);

	// データの読み込み
	Load();

	// スコアを生成する
	m_pResultScore = CResultScore::Create(D3DXVECTOR3(800.0f, 450.0f, 0.0f), 270.0f, 50.0f,0,m_nGameScore);

	// サウンド取得
	CSound* pSound = CManager::GetSound();

	// nullだったら
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	pSound->PlaySound(CSound::SOUND_LABEL_RESULTBGM);

	// 初期化結果を返す
	return S_OK;
}
//=================================
// 終了処理
//=================================
void CResultManager::Uninit(void)
{
	// 無し
}
//=================================
// 更新処理
//=================================
void CResultManager::Update(void)
{
	// 入力デバイスを取得
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	CInputJoypad* pJoyPad = CManager::GetJoyPad();

	// 取得失敗時
	if (pInput == nullptr) return;
	if (pJoyPad == nullptr) return;

	// カメラを取得
	CCamera* pCamera = CManager::GetCamera();

	// 取得失敗時
	if (pCamera == nullptr) return;

	// 決定キー or Aボタン 
	if ((pInput->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(CInputJoypad::JOYKEY_A)))
	{
		// ポインタ取得
		CFade* pFade = CManager::GetFade();

		// nullじゃないとき
		if (pFade != nullptr)
		{
			// スコアを書き出す
			m_pResultScore->Save();

			// シーン遷移
			pFade->SetFade(new CRanking());

			return;
		}
	}
}
//=================================
// 読み込み処理
//=================================
void CResultManager::Load(void)
{
	// 読み取った値を格納するメンバ変数
	m_nGameScore = NULL;

	//==============================
	// GameScore.txt
	//==============================
	std::ifstream file("data\\SCORE\\GameScore.txt");

	if (file.is_open())
	{
		file >> m_nGameScore;	// 数値1個を読み取り
		file.close();
	}
	else
	{
		MessageBox(NULL, "GameScore.txt が開けませんでした", "エラー", MB_OK);
	}
}
