//==========================================================
//
// カメラアニメーション時に適用するUI処理 [ moveui.cpp ]
// Author: Asuma Nishio
//
//==========================================================

//**********************
// インクルードファイル
//**********************
#include "moveui.h"
#include "manager.h"
#include "texture.h"

//**********************
// 名前空間
//**********************
namespace MOVEUIINFO
{
	constexpr float MOVESPEED_X = 12.0f;
	constexpr float MOVESPEED_Y = 15.0f;
};

//=================================
// オーバーロードコンストラクタ
//=================================
CMoveUi::CMoveUi(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_nIdxTexture = NULL;
	m_nMoveType = MOVETYPE_NONE;
	m_nScaleType = SCALETYPE_NONE;
}
//=================================
// デストラクタ
//=================================
CMoveUi::~CMoveUi()
{
	// 無し
}
//=================================
// 生成処理処理
//=================================
CMoveUi* CMoveUi::Create(D3DXVECTOR3 pos, float fWidth,float fHeight, const char * pTexName,MOVETYPE movetype,SCALETYPE sceletype)
{
	// インスタンス生成
	CMoveUi* pMoveUi = new CMoveUi;
	if (pMoveUi == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pMoveUi->Init()))
	{
		return nullptr;
	}

	// 2Dオブジェクト設定
	pMoveUi->SetPos(pos);
	pMoveUi->SetSize(fWidth, fHeight);
	pMoveUi->SetTexture(pTexName);
	pMoveUi->m_nMoveType = movetype;
	pMoveUi->m_nScaleType = sceletype;
	pMoveUi->SetAnchor(CObject2D::ANCHORTYPE_CENTER);
	pMoveUi->SetDrawType(0);

	// 生成されたポインタ
	return pMoveUi;
}
//=================================
// 初期化処理
//=================================
HRESULT CMoveUi::Init(void)
{
	// 親クラスの初期化処理
	CObject2D::Init();

	// 初期化結果を返す
	return S_OK;
}
//=================================
// 終了処理
//=================================
void CMoveUi::Uninit(void)
{
	// 親クラスの終了処理
	CObject2D::Uninit();
}
//=================================
// 更新処理
//=================================
void CMoveUi::Update(void)
{
	// 現在座標を取得
	D3DXVECTOR3 NowPos = GetPos();

	// 画面中央座標
	float fCenterPos = SCREEN_WIDTH * 0.5f;
	float fCenterPosHeight = SCREEN_HEIGHT * 0.5f;

	switch (m_nMoveType)
	{
	case MOVETYPE_NONE:
		break;

	case MOVETYPE_RIGHT: // 右から中央へ

		if (NowPos.x > fCenterPos)
		{
			NowPos.x -= MOVEUIINFO::MOVESPEED_X;

			if (NowPos.x <= fCenterPos)
			{
				NowPos.x = fCenterPos;
				m_nMoveType = MOVETYPE_CENTER; // 到達したら中央固定
			}
			SetPos(NowPos);
		}

		break;

	case MOVETYPE_LEFT:   // 左から中央へ

		if (NowPos.x < fCenterPos)
		{
			NowPos.x += MOVEUIINFO::MOVESPEED_X;

			if (NowPos.x >= fCenterPos)
			{
				NowPos.x = fCenterPos;
				m_nMoveType = MOVETYPE_CENTER; // 到達したら中央固定
			}

			SetPos(NowPos);
		}
		break;

	case MOVETYPE_CENTER: // 中央で止まる

		NowPos.x = fCenterPos;
		SetPos(NowPos);

		break;

	case MOVETYPE_DOWN:   // 上から下

		if (NowPos.y < fCenterPosHeight)
		{
			NowPos.y += MOVEUIINFO::MOVESPEED_Y;

			if (NowPos.y >= fCenterPosHeight)
			{
				NowPos.x = fCenterPosHeight;
				m_nMoveType = MOVETYPE_CENTER; // 到達したら中央固定
			}

			SetPos(NowPos);
		}
		break;

	}

	switch (m_nScaleType)
	{
	case CMoveUi::SCALETYPE_NONE:
		break;

	case CMoveUi::SCALETYPE_CENTER: // 中央から
	{
		// 最大幅
		const float fMaxWidth = 150.0f;		// 横幅

		// 拡大スピード
		const float fAddWidth = 3.0f;		// フレーム毎の加算量

		// 現在の幅
		float fWidth = GetWidth();

		if (fWidth < fMaxWidth)
		{
			fWidth += fAddWidth;

			if (fWidth >= fMaxWidth)
			{
				fWidth = fMaxWidth;
				m_nScaleType = SCALETYPE_LEFTDOWN; //左に移動する
			}

			SetWidth(fWidth);
		}
	}
	break;

	case SCALETYPE_LEFTDOWN:
	{
		// 目標座標
		D3DXVECTOR3 DestPos = { 135.0f, 600.0f, 0.0f };

		const float fDestHeight = 40.0f;	// 高さ

		// 移動スピード
		const float fLerpSpeed = 0.05f;

		// 現在値取得
		float fHeight = GetHeight();

		// 線形補間
		NowPos.x += (DestPos.x - NowPos.x) * fLerpSpeed;
		NowPos.y += (DestPos.y - NowPos.y) * fLerpSpeed;

		// 高さを徐々に縮小
		fHeight += (fDestHeight - fHeight) * fLerpSpeed;

		SetPos(NowPos);
		SetHeight(fHeight);
		SetWidth(140.0f);

		// 終了判定
		if (fabs(DestPos.x - NowPos.x) < 1.0f &&
			fabs(DestPos.y - NowPos.y) < 1.0f &&
			fabs(fDestHeight - fHeight) < 1.0f)
		{
			SetPos(DestPos);		// オブジェクトセット
			SetHeight(fDestHeight);	// オブジェクトセット
			m_nScaleType = SCALETYPE_NONE;
		}	
	}
		break;

	default:
		break;
	}

	// 親クラス更新処理
	CObject2D::Update();
}
//=================================
// 描画処理
//=================================
void CMoveUi::Draw(void)
{
	// オブジェクトの描画
	CObject2D::Draw();
}
