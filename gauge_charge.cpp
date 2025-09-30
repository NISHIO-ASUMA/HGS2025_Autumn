//==============================
//
//  タイマー処理[.cpp]
//  Author : Kaiti Aoto
//
//==============================
#include "gauge_charge.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "object2D.h"
#include "input.h"
#include "game.h"
//==================
// コンストラクタ
//==================
CChargeGauge::CChargeGauge(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Base = 0;
	m_fHeight = 0;
}
//================
// デストラクタ
//================
CChargeGauge::~CChargeGauge()
{
}
//===========
// 生成処理
//===========
CChargeGauge* CChargeGauge::Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	CChargeGauge* pGause;
	//
	pGause = new CChargeGauge;

	pGause->m_pos = pos;
	pGause->m_Base = base;
	pGause->m_fHeight = fHeight;
	pGause->m_col = col;


	//初期化
	if (FAILED(pGause->Init()))
	{
		delete pGause;
		return nullptr;
	}

	return pGause;
}
//=============
// 初期化処理
//=============
HRESULT CChargeGauge::Init(void)
{
	m_bUse = false;
	m_pGauge = CGauge::Create(m_pos, m_Base, m_fHeight, m_col);

	return S_OK;
}
//==========
// 終了処理
//==========
void CChargeGauge::Uninit(void)
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
void CChargeGauge::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// キーボードの取得
	CInputJoypad* pJoypad = CManager::GetJoyPad();				// ジョイパッドの取得

	//プレイヤー情報取得
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	if (pKeyboard->GetPress(DIK_SPACE) || pJoypad->GetPressR2())
	{
		pPlayer->ChargeShot();

		m_bUse = true;
		m_nCntReset++;
		m_Base += 0.05f;
		if (m_nCntReset >= PLAYER_CHARGETIME)
		{//一定時間経過で中身を０に
			pPlayer->ChargeShot();
			m_nCntReset = 0;
			m_bUse = false;
			m_Base = 0;
		}
	}
	else
	{
		m_nCntReset = 0;
		m_Base = 0;
		m_bUse = false;
	}
	m_pGauge->SetBase(m_Base);
	m_pGauge->Set();
}
//===========
// 描画処理
//===========
void CChargeGauge::Draw(void)
{
	if (m_bUse == true)
	{
		m_pGauge->Draw();
	}
}