//=======================================
//
// リザルトスコア処理 [ resultscore.cpp ]
// Author: Asuma Nishio
// 
//=======================================

//**********************
// インクルードファイル
//**********************
#include "resultscore.h"
#include "rankingscore.h"
#include "manager.h"
#include "texture.h"
#include "number.h"
#include <algorithm>

//**********************
// 名前空間
//**********************
namespace RESULTSCOREINFO
{
	constexpr int MAXSCORE = 40000; // 最大スコア
	constexpr float MAX_VALUESCORE = 120.0f; // 計算割合
	constexpr float SCALE = 20.0f;
	constexpr float MOVE = 0.7f;
	constexpr float MINSCALE = 1.0f;
	const char* FILENAME = "data\\SCORE\\RankScore.txt"; // ファイルパス
};

//================================
// コンストラクタ
//================================
CResultScore::CResultScore(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_nScore = NULL;
	m_nLastScore = NULL;
	m_nTimeScore = NULL;
	m_nIdxTexture = NULL;
	m_nDestLastScore = NULL;
	m_nDestTimeScore = NULL;
	m_nLastScore = NULL;

	m_fScale = NULL;		// 最初は大きく
	m_fTargetScale = NULL;	// 通常サイズ
	m_fScaleSpeed = NULL;	// 縮小速度

	m_pos = VECTOR3_NULL;
	m_DestPos = VECTOR3_NULL;

	for (int nCnt = 0; nCnt < NUM_RESULTSCORE; nCnt++)
	{
		m_apNumber[nCnt] = nullptr;
	}
}
//================================
// デストラクタ
//================================
CResultScore::~CResultScore()
{
	// 無し
}
//================================
// 生成処理
//================================
CResultScore* CResultScore::Create(D3DXVECTOR3 pos, float fWidth, float fHeight,int nType,int nDestScore)
{
	// インスタンス生成
	CResultScore* pResultScore = new CResultScore;
	if (pResultScore == nullptr) return nullptr;

	// 座標,サイズ設定
	pResultScore->m_DestPos = pos; // 目的の座標

	// 左上からスタート
	const float fOffsetX = -400.0f; // 左
	const float fOffsetY = -800.0f; // 上
	pResultScore->m_pos = pos + D3DXVECTOR3(fOffsetX, fOffsetY, 0.0f);

	// 値セット
	pResultScore->m_fWidth = fWidth;
	pResultScore->m_fHeight = fHeight;
	pResultScore->m_nType = nType;
	pResultScore->m_nScore = nDestScore;

	// 初期化失敗時
	if (FAILED(pResultScore->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pResultScore;
}
//================================
// 初期化処理
//================================
HRESULT CResultScore::Init(void)
{
	// 横幅計算
	float fTexPos = m_fWidth / NUM_RESULTSCORE;

	// 桁数分
	for (int nCnt = 0; nCnt < NUM_RESULTSCORE; nCnt++)
	{
		// インスタンス生成
		m_apNumber[nCnt] = new CNumber;

		// 初期化処理
		m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);

		// 座標設定
		m_apNumber[nCnt]->SetPos(m_pos);

		// ナンバー変数のサイズ
		m_apNumber[nCnt]->SetSize(fTexPos, m_fHeight);

		// テクスチャセット
		m_apNumber[nCnt]->SetTexture("score000.png");
	}

	m_fScale = RESULTSCOREINFO::SCALE;			// 最初は大きく
	m_fTargetScale = RESULTSCOREINFO::MINSCALE;	// 通常サイズ
	m_fScaleSpeed = RESULTSCOREINFO::MOVE;		// 縮小速度

	// 初期化結果を返す
	return S_OK;
}
//================================
// 終了処理
//================================
void CResultScore::Uninit(void)
{
	// 使った分破棄
	for (int nCnt = 0; nCnt < NUM_RESULTSCORE; nCnt++)
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
//================================
// 更新処理
//================================
void CResultScore::Update(void)
{	
	// 減算処理
	if (m_fScale > m_fTargetScale)
	{
		// 減少
		m_fScale -= m_fScaleSpeed;

		// 下回ったら
		if (m_fScale < m_fTargetScale)
			m_fScale = m_fTargetScale;
	}	
	
	// 座標のイージング補間
	float fEase = 0.2f; // 補間の値
	m_pos.x += (m_DestPos.x - m_pos.x) * fEase;
	m_pos.y += (m_DestPos.y - m_pos.y) * fEase;

	// スコア格納
	int nScore = m_nScore;

	// 八桁分
	for (int nCntScore = 0; nCntScore < NUM_RESULTSCORE; nCntScore++) // 右から処理
	{
		// 桁数ごとに分割する値を計算
		int nDigit = nScore % NUM_RESULTDIGIT;

		nScore /= NUM_RESULTDIGIT;

		// ナンバー更新
		m_apNumber[nCntScore]->Update();

		// 桁更新
		m_apNumber[nCntScore]->SetDigit(nDigit);

		// 座標を更新
		D3DXVECTOR3 Pos(
			m_pos.x - ((m_fWidth / NUM_RESULTSCORE) * 2.0f * nCntScore * m_fScale),
			m_pos.y,
			0.0f);
		m_apNumber[nCntScore]->SetPos(Pos);

		// サイズ更新
		float fTexPos = (m_fWidth / NUM_RESULTSCORE) * m_fScale;
		float fHeight = m_fHeight * m_fScale;
		m_apNumber[nCntScore]->SetSize(fTexPos, fHeight);
	}
}
//================================
// 描画処理
//================================
void CResultScore::Draw(void)
{
	// 使っている桁数分の描画
	for (int nCnt = 0; nCnt < NUM_RESULTSCORE; nCnt++)
	{
		// ナンバー描画
		m_apNumber[nCnt]->Draw();
	}
}
//================================
// 書き出し処理
//================================
void CResultScore::Save(void)
{
	// 最大5件
	int nScores[NUM_RESULTSCORE] = { NULL };

	// 読み込み件数
	int nCnt = NULL;

	// ファイルから読み込み
	std::ifstream inFile(RESULTSCOREINFO::FILENAME);

	// ファイルが開けたら
	if (inFile.is_open())
	{
		// 5件分読み込む
		while (nCnt < NUM_RESULTSCORE && inFile >> nScores[nCnt])
		{
			nCnt++;
		}

		// ファイルを閉じる
		inFile.close();
	}

	// 今回のスコアをランキングに入れるか判定
	if (nCnt < NUM_RESULTSCORE)
	{
		nScores[nCnt++] = m_nScore;
	}
	else
	{
		// 最小値を探す
		int nMinData = 0;

		// 1番目から
		for (int nScoreData = 1; nScoreData < NUM_RESULTSCORE; nScoreData++)
		{
			// スコアの値が低いなら
			if (nScores[nScoreData] < nScores[nMinData])
			{
				// 配列番号に代入
				nMinData = nScoreData;
			}
		}

		// 新スコアが最小値より大きければ差し替え
		if (m_nScore > nScores[nMinData])
		{
			// スコアをセットする
			nScores[nMinData] = m_nScore;
		}
	}

	// ファイルに書き出し
	std::ofstream outFile(RESULTSCOREINFO::FILENAME);

	// 開けたら
	if (outFile.is_open())
	{
		// 先にソートを実行
		std::sort(nScores, nScores + NUM_RESULTSCORE, std::greater<int>());

		// ソート後に書き出す
		for (int nCntScore = 0; nCntScore < NUM_RESULTSCORE; nCntScore++)
		{
			outFile << nScores[nCntScore] << std::endl;
		}

		// ファイルを閉じる
		outFile.close();
	}
	else
	{
		// 例外メッセージ
		MessageBox(NULL, "RankScore.txt が開けませんでした", "エラー", MB_OK);
	}

	for (int nCntSave = 0; nCntSave < NUM_RESULTSCORE; nCntSave++)
	{
		// 該当スコアと一致していたら
		if (nScores[nCntSave] == m_nScore)
		{
			// 最終的な順位を記録
			CRankingScore::SetRankInScoreIdx(nCntSave);

			break;
		}
	}
}
//================================
// テクスチャ処理
//================================
void CResultScore::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// テクスチャ割り当て
	m_nIdxTexture = pTexture->RegisterDynamic("data\\TEXTURE\\Score000.png");
}
