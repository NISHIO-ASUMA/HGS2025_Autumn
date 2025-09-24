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
#include "ui.h"
#include "manager.h"
#include "ranking.h"

//=================================
// �I�[�o�[���[�h�R���X�g���N�^
//=================================
CResult::CResult() : CScene(CScene::MODE_RESULT)
{

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
	// ui����
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "result.jpg", false);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//=================================
// �I������
//=================================
void CResult::Uninit(void)
{
	// ����
}
//=================================
// �X�V����
//=================================
void CResult::Update(void)
{
	// �L�[����
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		CFade* pFade = CManager::GetFade();

		if (pFade != nullptr)
		{
			pFade->SetFade(new CRanking());

			return;
		}
	}

}
//=================================
// �`�揈��
//=================================
void CResult::Draw(void)
{
	// ����
}
