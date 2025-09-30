//==============================
//
//  �^�C�}�[����[.cpp]
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
// �R���X�g���N�^
//==================
CChargeGauge::CChargeGauge(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Base = 0;
	m_fHeight = 0;
}
//================
// �f�X�g���N�^
//================
CChargeGauge::~CChargeGauge()
{
}
//===========
// ��������
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


	//������
	if (FAILED(pGause->Init()))
	{
		delete pGause;
		return nullptr;
	}

	return pGause;
}
//=============
// ����������
//=============
HRESULT CChargeGauge::Init(void)
{
	m_bUse = false;
	m_pGauge = CGauge::Create(m_pos, m_Base, m_fHeight, m_col);

	return S_OK;
}
//==========
// �I������
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
// �X�V����
//==========
void CChargeGauge::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
	CInputJoypad* pJoypad = CManager::GetJoyPad();				// �W���C�p�b�h�̎擾

	//�v���C���[���擾
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	if (pKeyboard->GetPress(DIK_SPACE) || pJoypad->GetPressR2())
	{
		pPlayer->ChargeShot();

		m_bUse = true;
		m_nCntReset++;
		m_Base += 0.05f;
		if (m_nCntReset >= PLAYER_CHARGETIME)
		{//��莞�Ԍo�߂Œ��g���O��
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
// �`�揈��
//===========
void CChargeGauge::Draw(void)
{
	if (m_bUse == true)
	{
		m_pGauge->Draw();
	}
}