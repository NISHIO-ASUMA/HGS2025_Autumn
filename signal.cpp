//=====================================
//
// �U���T�C������ [ signal.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "signal.h"

//**********************
// ���O���
//**********************
namespace SIGNINFO
{
	inline constexpr int LIFE = 120; // �ő����
	inline constexpr float UIHEIGHT = 50.0f;	// UI�T�C�Y
	inline constexpr float UIWIDTH = 250.0f;	// UI�T�C�Y
	const D3DXVECTOR3 ACTIVEPOS = { 640.0f,360.0f,0.0f }; // UI���W
};

//==============================
// �R���X�g���N�^
//==============================
CSignal::CSignal(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nLife = NULL;
}
//==============================
// �f�X�g���N�^
//==============================
CSignal::~CSignal()
{
	// ����
}
//==============================
// ��������
//==============================
CSignal* CSignal::Create(void)
{
	// �C���X�^���X����
	CSignal* pSign = new CSignal;
	if (pSign == nullptr) return nullptr;

	// �I�u�W�F�N�g�ݒ�
	pSign->SetPos(SIGNINFO::ACTIVEPOS);
	pSign->SetSize(SIGNINFO::UIWIDTH, SIGNINFO::UIHEIGHT);
	pSign->SetAnchor(ANCHORTYPE_CENTER);
	pSign->SetTexture("warning.png");

	// ���������s��
	if (FAILED(pSign->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pSign;
}
//==============================
// ����������
//==============================
HRESULT CSignal::Init(void)
{
	// �e�N���X�̏���������
	CObject2D::Init();

	// ������ݒ�
	m_nLife = SIGNINFO::LIFE;

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==============================
// �I������
//==============================
void CSignal::Uninit(void)
{
	// �e�N���X�̏I������
	CObject2D::Uninit();
}
//==============================
// �X�V����
//==============================
void CSignal::Update(void)
{
	// �_�ŏ��������s
	SetFlash(15, 30, COLOR_WHITE);

	// �̗͂����炷
	m_nLife--;

	// 0�ȉ�
	if (m_nLife <= NULL)
	{
		// �I������
		Uninit();

		// �����ŏ����I��
		return;
	}

	// �e�N���X�̍X�V����
	CObject2D::Update();
}
//==============================
// �`�揈��
//==============================
void CSignal::Draw(void)
{
	// �e�N���X�̕`��
	CObject2D::Draw();
}
