//=====================================
//
// ���C���Q�[������ [ game.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "game.h"
#include "manager.h"
#include "debugproc.h"
#include "title.h"
#include "result.h"
#include "ui.h"

//**************************
// �ÓI�����o�ϐ��錾
//**************************
CPauseManager* CGame::m_pPausemanager = nullptr; // �|�[�Y�}�l�[�W���[�̃|�C���^

//==================================
// �R���X�g���N�^
//==================================
CGame::CGame() : CScene(CScene::MODE_GAME)
{
	// �l�̃N���A
	m_nGametype = GAMESTATE_NONE;
	m_nStateCount = NULL;
}
//==================================
// �f�X�g���N�^
//==================================
CGame::~CGame()
{
	// ����
}
//==================================
// ����������
//==================================
HRESULT CGame::Init(void)
{
	// �|�[�Y�}�l�[�W���[����
	m_pPausemanager = new CPauseManager;

	// null��������
	if (m_pPausemanager == nullptr)
	{
		return E_FAIL;
	}

	// �|�[�Y�}�l�[�W���[����������
	m_pPausemanager->Init();

	// �ʏ�i�s���
	m_nGametype = GAMESTATE_NORMAL;

	// ui����
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "game.jpg", false);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==================================
// �I������
//==================================
void CGame::Uninit(void)
{
	// null�`�F�b�N
	if (m_pPausemanager != nullptr)
	{
		// �I������
		m_pPausemanager->Uninit();

		// �|�C���^�̔j��
		delete m_pPausemanager;

		// nullptr�ɂ���
		m_pPausemanager = nullptr;
	}

}
//==================================
// �X�V����
//==================================
void CGame::Update(void)
{	
	// �|�[�Y�̃L�[���͔���
	m_pPausemanager->SetEnablePause();
	
	// �|�[�Y�̍X�V����
	m_pPausemanager->Update();
	
	// �t�F�[�h�擾
	CFade* pFade = CManager::GetFade();

	switch (m_nGametype)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;

	case GAMESTATE_END:
		m_nStateCount++;

		if (m_nStateCount >= 30)
		{
			// �J�E���^�[��������
			m_nStateCount = 0;
			
			// 1�b�o��
			m_nGametype = GAMESTATE_NONE;//�������Ă��Ȃ����

			// �t�F�[�h���擾�ł�����
			if (pFade != nullptr)
			{
				// ���U���g�V�[���ɑJ��
				pFade->SetFade(new CResult());

				// �����ŏ�����Ԃ�
				return;
			}
		}
		break;

	case GAMESTATE_LOSEEND:
		m_nStateCount++;

		if (m_nStateCount >= 30)
		{
			// �J�E���^�[��������
			m_nStateCount = 0;

			// 1�b�o��
			m_nGametype = GAMESTATE_NONE;//�������Ă��Ȃ����

			// �t�F�[�h���擾�ł�����
			if (pFade != nullptr)
			{
				// ���U���g�V�[���ɑJ��
				pFade->SetFade(new CResult());

				// �����ŏ�����Ԃ�
				return;
			}
		}
		break;
	}

	// false�̎��ɍX�V
	if (m_pPausemanager->GetPause() == false)
	{
		// �L�[����
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
		{
			if (pFade != nullptr)
			{
				pFade->SetFade(new CResult());

				return;
			}
		}
	}
}
//==================================
// �`�揈��
//==================================
void CGame::Draw(void)
{
	// ����
}
//==================================
// �R���X�g���N�^
//==================================
CGame* CGame::Create(void)
{	
	// �C���X�^���X����
	CGame* pGame = new CGame;

	// nullptr��������
	if (pGame == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pGame->Init()))
	{
		// �|�C���^��Ԃ�
		return nullptr;
	}

	// �������ꂽ�|�C���^��Ԃ�
	return pGame;
}