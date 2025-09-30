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
#include "player.h"
#include "score.h"

//**************************
// �ÓI�����o�ϐ��錾
//**************************
CPauseManager* CGame::m_pPausemanager = nullptr; // �|�[�Y�}�l�[�W���[�̃|�C���^
CBlockManager* CGame::m_pBlockManager = nullptr; // �u���b�N�}�l�[�W���[�̃|�C���^
CTime* CGame::m_pTime = nullptr;				 // �^�C���N���X�̃|�C���^

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

	// �u���b�N�}�l�[�W���[�̐���
	m_pBlockManager = new CBlockManager;

	// �u���b�N�}�l�[�W���[�̏�����
	m_pBlockManager->Init();

	// JSON�̓ǂݍ���
	m_pBlockManager->LoadFromJson("data/test_01.json");

	// �ʏ�i�s���
	m_nGametype = GAMESTATE_NORMAL;

	// �v���C���[�̐���
	CPlayer* player = CCharacterManager::GetInstance().AddCharacter<CPlayer>();

	// �L�����N�^�[�̏���������
	CCharacterManager::GetInstance().InitAll();

	// �^�C�}�[����
	m_pTime = CTime::Create(D3DXVECTOR3(560.0f, 40.0f, 0.0f), 60.0f, 40.0f);
	
	// �R��������
	CUi::Create(D3DXVECTOR3(665.0f, 40.0f, 0.0f), 0, 15.0f, 25.0f, "coron.png", false);

	// �X�R�A����
	CScore::Create(D3DXVECTOR3(1240.0f, 660.0f, 0.0f), 180.0f, 60.0f);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==================================
// �I������
//==================================
void CGame::Uninit(void)
{
	// �L�����N�^�[�̏I������
	CCharacterManager::GetInstance().Uninit();

	// �u���b�N�}�l�[�W���[�̔j��
	if (m_pBlockManager != nullptr)
	{
		m_pBlockManager->Uninit();

		delete m_pBlockManager;
		m_pBlockManager = nullptr;
	}

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
	
	// �u���b�N�}�l�[�W���[�̍X�V����
	m_pBlockManager->Update();

	// �t�F�[�h�擾
	CFade* pFade = CManager::GetFade();

	switch (m_nGametype)
	{
	case GAMESTATE_NORMAL://�ʏ���
		break;

	case GAMESTATE_END:
		m_nStateCount++;

		if (m_nStateCount >= 60)
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
		// �^�C����0�ȉ��̎�
		if (m_pTime->GetAllTime() == NULL)
		{
			m_nGametype = GAMESTATE_END;
		}

#ifdef _DEBUG
		// �L�[����
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
		{
			if (pFade != nullptr)
			{
				pFade->SetFade(new CResult());

				return;
			}
		}
#endif // _DEBUG
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