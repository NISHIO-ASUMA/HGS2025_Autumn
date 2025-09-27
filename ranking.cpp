//=======================================
//
// �����L���O�V�[������ [ ranking.cpp ]
// Author: Asuma Nishio
//
//=======================================

//******************************
// �C���N���[�h�t�@�C���錾
//******************************
#include "ranking.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "ui.h"

//===================================
// �I�[�o�[���[�h�R���X�g���N�^
//===================================
CRanking::CRanking() : CScene(CScene::MODE_RANKING)
{

}
//===================================
// �f�X�g���N�^
//===================================
CRanking::~CRanking()
{
	// ����
}
//===================================
// ��������
//===================================
CRanking* CRanking::Create(void)
{
	// �C���X�^���X����
	CRanking* pRanking = new CRanking;
	if (pRanking == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pRanking->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pRanking;
}
//===================================
// ����������
//===================================
HRESULT CRanking::Init(void)
{
	// ui����
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "ranking.jpg", false);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//===================================
// �I������
//===================================
void CRanking::Uninit(void)
{
	// ����
}
//===================================
// �X�V����
//===================================
void CRanking::Update(void)
{
	// ����L�[����
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetJoyPad()->GetTrigger(CInputJoypad::JOYKEY_A) || CManager::GetJoyPad()->GetTrigger(CInputJoypad::JOYKEY_START))
	{
		// �t�F�[�h�擾
		CFade* pFade = CManager::GetFade();
		if (pFade == nullptr) return;

		// ��ʑJ��
		pFade->SetFade(new CTitle(true));

		return;
	}
}
//===================================
// �`�揈��
//===================================
void CRanking::Draw(void)
{
	// ����
}