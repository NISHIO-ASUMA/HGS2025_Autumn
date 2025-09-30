//=====================================
//
// 攻撃サイン処理 [ signal.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "signal.h"

//**********************
// 名前空間
//**********************
namespace SIGNINFO
{
	inline constexpr int LIFE = 120; // 最大寿命
	inline constexpr float UIHEIGHT = 50.0f;	// UIサイズ
	inline constexpr float UIWIDTH = 250.0f;	// UIサイズ
	const D3DXVECTOR3 ACTIVEPOS = { 640.0f,360.0f,0.0f }; // UI座標
};

//==============================
// コンストラクタ
//==============================
CSignal::CSignal(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nLife = NULL;
}
//==============================
// デストラクタ
//==============================
CSignal::~CSignal()
{
	// 無し
}
//==============================
// 生成処理
//==============================
CSignal* CSignal::Create(void)
{
	// インスタンス生成
	CSignal* pSign = new CSignal;
	if (pSign == nullptr) return nullptr;

	// オブジェクト設定
	pSign->SetPos(SIGNINFO::ACTIVEPOS);
	pSign->SetSize(SIGNINFO::UIWIDTH, SIGNINFO::UIHEIGHT);
	pSign->SetAnchor(ANCHORTYPE_CENTER);
	pSign->SetTexture("warning.png");

	// 初期化失敗時
	if (FAILED(pSign->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pSign;
}
//==============================
// 初期化処理
//==============================
HRESULT CSignal::Init(void)
{
	// 親クラスの初期化処理
	CObject2D::Init();

	// 寿命を設定
	m_nLife = SIGNINFO::LIFE;

	// 初期化結果を返す
	return S_OK;
}
//==============================
// 終了処理
//==============================
void CSignal::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//==============================
// 更新処理
//==============================
void CSignal::Update(void)
{
	// 点滅処理を実行
	SetFlash(15, 30, COLOR_WHITE);

	// 体力を減らす
	m_nLife--;

	// 0以下
	if (m_nLife <= NULL)
	{
		// 終了処理
		Uninit();

		// ここで処理終了
		return;
	}

	// 親クラスの更新処理
	CObject2D::Update();
}
//==============================
// 描画処理
//==============================
void CSignal::Draw(void)
{
	// 親クラスの描画
	CObject2D::Draw();
}
