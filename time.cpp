//====================================
//
// タイム処理 [ time.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "time.h"
#include "number.h"
#include "manager.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CTime::CTime(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_pos = VECTOR3_NULL;
	m_fHeight = NULL;
	m_fWidth = NULL;
	m_nAllTime = NULL;
	m_nSecond = NULL;
	m_nMinute = NULL;
	m_nDecTime = NULL;
	m_nCount = NULL;

	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		m_pNumberMinute[nCnt] = nullptr;
		m_pNumberSecond[nCnt] = nullptr;
	}
}
//===============================
// デストラクタ
//===============================
CTime::~CTime()
{
	// 無し
}
//===============================
// 生成処理
//===============================
CTime* CTime::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// タイムクラスのインスタンス生成
	CTime* pTime = new CTime;

	// nullptrだったら
	if (pTime == nullptr) return nullptr;

	// メンバ変数にセット
	pTime->m_fHeight = fHeight;
	pTime->m_fWidth = fWidth;
	pTime->m_pos = pos;

	// 初期化失敗時
	if (FAILED(pTime->Init()))
	{
		// nullptr代入
		return nullptr;
	}

	// 結果を返す
	return pTime;
}
//===============================
// 初期化処理
//===============================
HRESULT CTime::Init(void)
{
	// メンバ変数の初期化
	m_nAllTime = NUMTIME;

	// 分を計算
	m_nMinute = m_nAllTime / CARVETIME;

	// 秒を計算
	m_nSecond = m_nAllTime % CARVETIME;

	// 横幅計算
	float fTexPos = m_fWidth / DIGIT_TIME;

	// 分生成
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// インスタンス生成
		m_pNumberMinute[nCnt] = new CNumber;

		// ナンバー変数の初期化
		m_pNumberMinute[nCnt]->SetPos(m_pos);
		m_pNumberMinute[nCnt]->Init(D3DXVECTOR3(m_pos.x + (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);
		m_pNumberMinute[nCnt]->SetSize(fTexPos, m_fHeight);
		m_pNumberMinute[nCnt]->SetTexture("number003.png");
	}

	// ずらす値
	m_pos.x += VALUE_WIDTH;

	// 秒生成
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// インスタンス生成
		m_pNumberSecond[nCnt] = new CNumber;

		// ナンバー変数の初期化
		m_pNumberSecond[nCnt]->SetPos(m_pos);
		m_pNumberSecond[nCnt]->Init(D3DXVECTOR3(m_pos.x + (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);
		m_pNumberSecond[nCnt]->SetSize(fTexPos, m_fHeight);
		m_pNumberSecond[nCnt]->SetTexture("number003.png");
	}

	// 結果を返す
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CTime::Uninit(void)
{
	// 使った分破棄
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// nullptrチェック
		if (m_pNumberMinute[nCnt] != nullptr)
		{
			// 終了処理
			m_pNumberMinute[nCnt]->Uninit();

			// ポインタの破棄
			delete m_pNumberMinute[nCnt];

			// null初期化
			m_pNumberMinute[nCnt] = nullptr;
		}

		// nullptrチェック
		if (m_pNumberSecond[nCnt] != nullptr)
		{
			// 終了処理
			m_pNumberSecond[nCnt]->Uninit();

			// ポインタの破棄
			delete m_pNumberSecond[nCnt];

			// null初期化
			m_pNumberSecond[nCnt] = nullptr;
		}
	}

	// オブジェクトの破棄
	CObject::Release();
}
//===============================
// 更新処理
//===============================
void CTime::Update(void)
{
	// カウンターを加算
	m_nCount++;

	// 1秒経過後 ( 秒減少 )
	if (m_nCount >= CARVETIME)
	{
		// カウンターを初期化する
		m_nCount = 0;

		// 全体時間を減らす
		m_nAllTime--;

		// 分
		m_nMinute = m_nAllTime / CARVETIME;

		// 秒を減らす
		m_nSecond = m_nAllTime % CARVETIME;

		// 0以下なら
		if (m_nAllTime <= 0) m_nAllTime = 0;

		// 減少していった時間を加算
		m_nDecTime++;
	}

	// 桁計算
	Second();
	Minute();
}
//===============================
// 描画処理
//===============================
void CTime::Draw(void)
{
	// 桁数分描画
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// 分描画
		m_pNumberMinute[nCnt]->Draw();

		// 秒描画
		m_pNumberSecond[nCnt]->Draw();
	}
}
//===============================
// タイマーデータ保存処理
//===============================
void CTime::Save(void)
{
#if 0
	// 開くファイルを定義
	std::ofstream File("data\\Loader\\LastTime.txt");

	// 例外処理
	if (!File.is_open())
	{
		MessageBox(NULL, "保存ファイルを開けませんでした", "エラー", MB_OK);

		return;
	}

	// 数値を出力する
	File << m_nDecTime;

	// ファイルを閉じる
	File.close();
#endif
}
//===============================
// 秒計算
//===============================
void CTime::Second(void)
{
	int aData = DIVIDE * DIVIDE;
	int aData2 = DIVIDE;

	// 分計算
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// 桁数計算
		int aPosTexU = m_nMinute % aData / aData2;
		aData /= DIVIDE;
		aData2 /= DIVIDE;

		// 分の桁数を更新する
		if (m_pNumberMinute[nCnt] != nullptr)
		{
			m_pNumberMinute[nCnt]->Update();
			m_pNumberMinute[nCnt]->SetDigit(aPosTexU);
		}

	}
}
//===============================
// 分計算
//===============================
void CTime::Minute(void)
{
	int aData = DIVIDE * DIVIDE;
	int aData2 = DIVIDE;

	// 秒計算
	for (int nCnt = 0; nCnt < DIGIT_TIME; nCnt++)
	{
		// 桁数計算
		int aPosTexU = m_nSecond % aData / aData2;
		aData /= DIVIDE;
		aData2 /= DIVIDE;

		// 秒の桁数を更新する
		if (m_pNumberSecond[nCnt] != nullptr)
		{
			m_pNumberSecond[nCnt]->Update();
			m_pNumberSecond[nCnt]->SetDigit(aPosTexU);
		}
	}
}