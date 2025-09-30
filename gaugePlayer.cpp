//==============================
//
//  タイマー処理[.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "gaugePlayer.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "texture.h"
#include "game.h"
#include "object2D.h"
//==================
// コンストラクタ
//==================
CHpGauge::CHpGauge(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Base = 0;
	m_fHeight = 0;
	m_pGauge = nullptr;
}
//================
// デストラクタ
//================
CHpGauge::~CHpGauge()
{
}
//===========
// 生成処理
//===========
CHpGauge* CHpGauge::Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	CHpGauge* pGause;
	//
	pGause = new CHpGauge;

	//初期化
	if (FAILED(pGause->Init()))
	{
		delete pGause;
		return nullptr;
	}

	// 値を代入
	pGause->m_Base = base;
	pGause->m_col = col;
	pGause->m_fHeight = fHeight;
	pGause->m_pos = pos;

	pGause->m_pGauge = CGauge::Create(pos, base, fHeight, col);

	return pGause;
}
//=============
// 初期化処理
//=============
HRESULT CHpGauge::Init(void)
{
	return S_OK;
}
//==========
// 終了処理
//==========
void CHpGauge::Uninit(void)
{
	if (m_pGauge != nullptr)
	{
		m_pGauge->Uninit();
		delete m_pGauge;
		m_pGauge = nullptr;
	}
	CObject::Release();
}
//==========
// 更新処理
//==========
void CHpGauge::Update(void)
{
	//プレイヤー情報取得
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	m_Base = (float)pPlayer->GetLife();

	m_rate = (PLAYER_LIFE > 0.0f) ? (m_Base / PLAYER_LIFE) : 0.0f;

	if (m_rate > 0.7f)
	{
		// 明るい水色
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (m_rate > 0.4f)
	{
		// オレンジ
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	}
	else if (m_rate > 0.1f)
	{
		// ピンク寄りの赤
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	m_pGauge->SetBase(m_Base);
	m_pGauge->SetRate(m_rate);
	m_pGauge->SetColor(m_col);
	m_pGauge->Set();
}
//===========
// 描画処理
//===========
void CHpGauge::Draw(void)
{
	m_pGauge->Draw();
}