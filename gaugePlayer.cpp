//==============================
//
//  �^�C�}�[����[.cpp]
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
// �R���X�g���N�^
//==================
CHpGauge::CHpGauge(int nPriority):CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Base = 0;
	m_fHeight = 0;
	m_pGauge = nullptr;
}
//================
// �f�X�g���N�^
//================
CHpGauge::~CHpGauge()
{
}
//===========
// ��������
//===========
CHpGauge* CHpGauge::Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col)
{
	CHpGauge* pGause;
	//
	pGause = new CHpGauge;

	//������
	if (FAILED(pGause->Init()))
	{
		delete pGause;
		return nullptr;
	}

	// �l����
	pGause->m_Base = base;
	pGause->m_col = col;
	pGause->m_fHeight = fHeight;
	pGause->m_pos = pos;

	pGause->m_pGauge = CGauge::Create(pos, base, fHeight, col);

	return pGause;
}
//=============
// ����������
//=============
HRESULT CHpGauge::Init(void)
{
	return S_OK;
}
//==========
// �I������
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
// �X�V����
//==========
void CHpGauge::Update(void)
{
	//�v���C���[���擾
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	m_Base = (float)pPlayer->GetLife();

	m_rate = (PLAYER_LIFE > 0.0f) ? (m_Base / PLAYER_LIFE) : 0.0f;

	if (m_rate > 0.7f)
	{
		// ���邢���F
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else if (m_rate > 0.4f)
	{
		// �I�����W
		m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	}
	else if (m_rate > 0.1f)
	{
		// �s���N���̐�
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	m_pGauge->SetBase(m_Base);
	m_pGauge->SetRate(m_rate);
	m_pGauge->SetColor(m_col);
	m_pGauge->Set();
}
//===========
// �`�揈��
//===========
void CHpGauge::Draw(void)
{
	m_pGauge->Draw();
}