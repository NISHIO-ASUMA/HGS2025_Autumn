//====================================
//
// ���U���g�V�[������ [ result.cpp ]
// Author: Asuma Nishio
// 
//=====================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "result.h"
#include "manager.h"
#include "ranking.h"
#include "resultmanager.h"

//=================================
// �I�[�o�[���[�h�R���X�g���N�^
//=================================
CResult::CResult() : CScene(CScene::MODE_RESULT)
{
	// �l�̃N���A
	m_pResultManager = nullptr;
}
//=================================
// �f�X�g���N�^
//=================================
CResult::~CResult()
{
	// ����
}
//=================================
// ��������
//=================================
CResult* CResult::Create(void)
{
	// �C���X�^���X����
	CResult* pResult = new CResult;

	// �����Ɏ��s������
	if (pResult == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pResult->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pResult;
}
//=================================
// ����������
//=================================
HRESULT CResult::Init(void)
{
	// �|�C���^����
	m_pResultManager = new CResultManager;
	if (m_pResultManager == nullptr) return E_FAIL;

	// ����������
	m_pResultManager->Init();

	// ���������ʂ�Ԃ�
	return S_OK;
}
//=================================
// �I������
//=================================
void CResult::Uninit(void)
{
	// null�`�F�b�N
	if (m_pResultManager != nullptr)
	{
		// �I������
		m_pResultManager->Uninit();

		// �|�C���^�̔j��
		delete m_pResultManager;

		// null�ɂ���
		m_pResultManager = nullptr;
	}
}
//=================================
// �X�V����
//=================================
void CResult::Update(void)
{
	// null�`�F�b�N
	if (m_pResultManager != nullptr)
	{
		// �X�V����
		m_pResultManager->Update();
	}
}
//=================================
// �`�揈��
//=================================
void CResult::Draw(void)
{
	// ����
}
