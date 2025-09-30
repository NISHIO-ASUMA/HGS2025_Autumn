//=====================================
//
// �`���[�g���A������ [ tutorial.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "tutorial.h"
#include "manager.h"
#include "sound.h"
#include "game.h"
#include "ui.h"

//===============================
// �I�[�o�[���[�h�R���X�g���N�^
//===============================
CTutorial::CTutorial() : CScene(CScene::MODE_TUTORIAL)
{

}
//===============================
// �f�X�g���N�^
//===============================
CTutorial::~CTutorial()
{
	// ����
}
//===============================
// ����������
//===============================
HRESULT CTutorial::Init(void)
{
	// �T�E���h�擾
	CSound* pSound = CManager::GetSound();

	// null��������
	if (pSound == nullptr) return E_FAIL;

	// �T�E���h�Đ�
	// pSound->PlaySound(CSound::SOUND_LABEL_TUTORIALBGM);

	// ui����
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "tutorial05.jpg", false);

	m_bFin = false;

	// ���������ʂ�Ԃ�
	return S_OK;
}
//===============================
// �I������
//===============================
void CTutorial::Uninit(void)
{
	// ����
}
//===============================
// �X�V����
//===============================
void CTutorial::Update(void)
{
	CInputJoypad* pJoypad = CManager::GetJoyPad();			// �W���C�p�b�h�̎擾

	// �L�[����
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(pJoypad->JOYKEY_A))
	{
		if (m_bFin == false)
		{
			CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "alert01.jpg", false);
			m_bFin = true;
			return;
		}
		else
		{
			CFade* pFade = CManager::GetFade();

			if (pFade != nullptr)
			{
				pFade->SetFade(new CGame());

				return;
			}
		}
	}
}
//===============================
// �`�揈��
//===============================
void CTutorial::Draw(void)
{
	// ����
}
//===============================
// ��������
//===============================
CTutorial* CTutorial::Create(void)
{
	// �C���X�^���X����
	CTutorial* pTutorial = new CTutorial;

	// �������s��
	if (pTutorial == nullptr) return nullptr;

	// �����������Ɏ��s������
	if (FAILED(pTutorial->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pTutorial;
}