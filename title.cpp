//====================================
//
// �^�C�g������ [ title.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "title.h"
#include "manager.h"
#include "game.h"
#include "ui.h"
#include "tutorial.h"

//=====================================
// �R���X�g���N�^
//=====================================
CTitle::CTitle(bool isFirst) : CScene(CScene::MODE_TITLE), m_isCreate(isFirst)
{
	//����
}
//=====================================
// �f�X�g���N�^
//=====================================
CTitle::~CTitle()
{
	// ����
}
//=====================================
// ����������
//=====================================
HRESULT CTitle::Init(void)
{
	// ����
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "title.jpg", false);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//=====================================
// �I������
//=====================================
void CTitle::Uninit(void)
{

}
//=====================================
// �X�V����
//=====================================
void CTitle::Update(void)
{
	// �L�[����
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		CFade* pFade = CManager::GetFade();
		if (pFade != nullptr)
		{
			pFade->SetFade(new CTutorial());

			return;
		}
	}

	// �R���g���[���[�擾
	if (CManager::GetJoyPad()->GetTrigger(CManager::GetJoyPad()->JOYKEY_BACK))
	{
		PostQuitMessage(0);
		return;
	}
}
//=====================================
// �`�揈��
//=====================================
void CTitle::Draw(void)
{
	// ����
}
//=====================================
// ��������
//=====================================
CTitle* CTitle::Create(bool isFirst)
{
	// �C���X�^���X����
	CTitle* pTitle = new CTitle(isFirst);

	// null��������
	if (pTitle == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pTitle->Init()))
	{
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pTitle;
}