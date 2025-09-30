//====================================
//
// スコア処理 [ score.h ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "score.h"
#include "manager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//**********************
// 静的メンバ変数宣言
//**********************
int CScore::m_nScore = NULL;	// 総スコアカウント用
int CScore::m_nDecCount = NULL; // 減算スコアカウント用

//==========================================
// コンストラクタ
//==========================================
CScore::CScore(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_nScore = NULL;
	m_nIdxTexture = NULL;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		m_apNumber[nCnt] = nullptr;
	}
}
//==========================================
// デストラクタ
//==========================================
CScore::~CScore()
{
	// 無し
}
//==========================================
// 生成処理
//==========================================
CScore* CScore::Create(D3DXVECTOR3 pos,float fWidth,float fHeight)
{
	// スコアインスタンス生成
	CScore* pScore = new CScore;

	// nullptrだったら
	if (pScore == nullptr) return nullptr;

	// 座標,サイズ設定
	pScore->m_pos = pos;
	pScore->m_fWidth = fWidth;
	pScore->m_fHeight = fHeight;

	// 初期化失敗時
	if (FAILED(pScore->Init()))
	{
		// nullptrを返す
		return nullptr;
	}
	
	// スコアポインタを返す
	return pScore;
}
//==========================================
// 初期化処理
//==========================================
HRESULT CScore::Init(void)
{
	// 横幅計算
	float fTexPos = m_fWidth / NUM_SCORE;

	// 桁数分
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// インスタンス生成
		m_apNumber[nCnt] = new CNumber;

		// 初期化処理
		m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);

		// ナンバー変数のサイズ
		m_apNumber[nCnt]->SetSize(fTexPos, m_fHeight);

		// 座標設定
		m_apNumber[nCnt]->SetPos(m_pos);

		// テクスチャセット
		m_apNumber[nCnt]->SetTexture("score000.png");
	}

	// 初期化結果を返す
	return S_OK;
}
//==========================================
// 終了処理
//==========================================
void CScore::Uninit(void)
{
	// 使った分破棄
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// nullptrチェック
		if (m_apNumber[nCnt] != nullptr)
		{
			// 終了
			m_apNumber[nCnt]->Uninit();

			// 破棄
			delete m_apNumber[nCnt];

			// ポインタ初期化
			m_apNumber[nCnt] = nullptr;
		}
	}

	// 自身の破棄
	CObject::Release();
}
//==========================================
// 更新処理
//==========================================
void CScore::Update(void)
{
	// スコア格納
	int nScore = m_nScore;

	// 八桁分
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++) // 右から処理
	{
		// 桁数ごとに分割する値を計算
		int nDigit = nScore % NUM_DIGIT;

		nScore /= NUM_DIGIT;

		// ナンバー更新
		m_apNumber[nCntScore]->Update();

		// 桁更新
		m_apNumber[nCntScore]->SetDigit(nDigit);
	}
}
//==========================================
// 描画処理
//==========================================
void CScore::Draw(void)
{
	// 使っている桁数分の描画
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// ナンバー描画
		m_apNumber[nCnt]->Draw();
	}
}
//==========================================
// スコア加算処理
//==========================================
void CScore::AddScore(int nValue)
{
	// スコア加算
	m_nScore += nValue;
}
//==========================================
// スコア初期化
//==========================================
void CScore::DeleteScore()
{
	m_nScore = 0;
}
//==========================================
// スコア減算処理
//==========================================
void CScore::DecScore(void)
{
	// 減算回数を記録
	m_nDecCount++;
}
//==========================================
// スコア書き出し処理
//==========================================
void CScore::SaveScore(void)
{
	// 開くファイルを定義
	std::ofstream File("data\\SCORE\\GameScore.txt");

	// 例外処理
	if (!File.is_open())
	{
		MessageBox(NULL, "保存ファイルを開けませんでした", "エラー", MB_OK);

		return;
	}

	// 数値を出力する
	File << m_nScore;

	// ファイルを閉じる
	File.close();
}
//==========================================
// テクスチャ設定
//==========================================
void CScore::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ割り当て
	m_nIdxTexture = pTexture->RegisterDynamic("data\\TEXTURE\\number.png");
}
