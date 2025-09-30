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
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, "tutorial00.jpg", false);
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.25f, SCREEN_HEIGHT * 0.5f, "tutorial03.jpg", false);

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
	// �L�[����
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		CFade* pFade = CManager::GetFade();

		if (pFade != nullptr)
		{
			pFade->SetFade(new CGame());

			return;
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