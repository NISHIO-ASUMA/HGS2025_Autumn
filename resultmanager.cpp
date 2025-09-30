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
	m_nBulletScore = NULL;
	m_nAllScore = NULL;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pResultScore[nCnt] = nullptr;
	}
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
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "ResultBack.jpg", false);

	// データの読み込み
	Load();

	// スコアを生成する ( 敵,時間,弾,合計 )
	m_pResultScore[0] = CResultScore::Create(D3DXVECTOR3(1190.0f, 200.0f, 0.0f), 300.0f, 60.0f, 0, m_nGameScore);
	m_pResultScore[1] = CResultScore::Create(D3DXVECTOR3(1190.0f, 350.0f, 0.0f), 300.0f, 60.0f, 0, m_nLastTime);
	m_pResultScore[2] = CResultScore::Create(D3DXVECTOR3(1190.0f, 500.0f, 0.0f), 300.0f, 60.0f, 0, m_nBulletScore);

	// 先に計算
	m_nAllScore = m_nGameScore + m_nLastTime + m_nBulletScore;

	// 最終スコアをセット
	m_pResultScore[3] = CResultScore::Create(D3DXVECTOR3(1190.0f, 650.0f, 0.0f), 400.0f, 100.0f, 0, m_nAllScore);

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
			// 最後のスコアを書き出す
			m_pResultScore[3]->Save();

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
	m_nLastTime = NULL;
	m_nBulletScore = NULL;

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

	//==============================
	// TimeScore.txt
	//==============================
	std::ifstream tfile("data\\SCORE\\TimeScore.txt");

	if (tfile.is_open())
	{
		tfile >> m_nLastTime;	// 数値1個を読み取り
		tfile.close();
	}
	else
	{
		MessageBox(NULL, "GameScore.txt が開けませんでした", "エラー", MB_OK);
	}

	// 計算する
	MathScore();

	//==============================
	// BulletScore.txt
	//==============================
	std::ifstream bfile("data\\SCORE\\BulletScore.txt");

	if (bfile.is_open())
	{
		bfile >> m_nBulletScore;	// 数値1個を読み取り
		bfile.close();
	}
	else
	{
		MessageBox(NULL, "GameScore.txt が開けませんでした", "エラー", MB_OK);
	}

	// 計算する
	m_nBulletScore = m_nBulletScore * 10000;
}
//===============================
// スコア計算
//===============================
void CResultManager::MathScore(void)
{
	// 読み込んだ値
	float fValue = static_cast<float>(m_nLastTime);

	// もし60なら
	if (m_nLastTime == ADDSCORETIME)
	{
		// 最大値をセットする (50万)
		m_nLastTime = MAX_VALUESCORE;
		return;
	}

	// 比率を算出
	float fRatio = 1.0f - (fValue /MAX_VALUESCORE);

	// 範囲内で割合を算出
	if (fRatio < 0.0f) fRatio = 0.0f;
	if (fRatio > 1.0f) fRatio = 1.0f;

	// スコア計算
	int nMathscore = (int)(MAX_VALUESCORE * fRatio);

	// 最後にセットする
	m_nLastTime = nMathscore;
}
