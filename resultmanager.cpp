//==============================================
//
// ���U���g�V�[���Ǘ����� [ resultmanager.cpp ]
// Author: Asuma Nishio
// 
//==============================================

//**************************
// �C���N���[�h�t�@�C���錾
//**************************
#include "resultmanager.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "title.h"
#include "resultscore.h"
#include "ui.h"
#include "ranking.h"

//=================================
// �R���X�g���N�^
//=================================
CResultManager::CResultManager()
{
	// �l�̃N���A
	m_isKeyDown = false;
	m_nGameScore = NULL;
	m_nLastTime = NULL;
	m_pResultScore = nullptr;
}
//=================================
// �f�X�g���N�^
//=================================
CResultManager::~CResultManager()
{
	// ����
}
//=================================
// ����������
//=================================
HRESULT CResultManager::Init(void)
{	
	// UI����
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "resultback.jpg", false);

	// �f�[�^�̓ǂݍ���
	Load();

	// �X�R�A�𐶐�����
	m_pResultScore = CResultScore::Create(D3DXVECTOR3(800.0f, 450.0f, 0.0f), 270.0f, 50.0f,0,m_nGameScore);

	// �T�E���h�擾
	CSound* pSound = CManager::GetSound();

	// null��������
	if (pSound == nullptr) return E_FAIL;

	// �T�E���h�Đ�
	pSound->PlaySound(CSound::SOUND_LABEL_RESULTBGM);

	// ���������ʂ�Ԃ�
	return S_OK;
}
//=================================
// �I������
//=================================
void CResultManager::Uninit(void)
{
	// ����
}
//=================================
// �X�V����
//=================================
void CResultManager::Update(void)
{
	// ���̓f�o�C�X���擾
	CInputKeyboard* pInput = CManager::GetInputKeyboard();
	CInputJoypad* pJoyPad = CManager::GetJoyPad();

	// �擾���s��
	if (pInput == nullptr) return;
	if (pJoyPad == nullptr) return;

	// �J�������擾
	CCamera* pCamera = CManager::GetCamera();

	// �擾���s��
	if (pCamera == nullptr) return;

	// ����L�[ or A�{�^�� 
	if ((pInput->GetTrigger(DIK_RETURN) || pJoyPad->GetTrigger(CInputJoypad::JOYKEY_A)))
	{
		// �|�C���^�擾
		CFade* pFade = CManager::GetFade();

		// null����Ȃ��Ƃ�
		if (pFade != nullptr)
		{
			// �X�R�A�������o��
			m_pResultScore->Save();

			// �V�[���J��
			pFade->SetFade(new CRanking());

			return;
		}
	}
}
//=================================
// �ǂݍ��ݏ���
//=================================
void CResultManager::Load(void)
{
	// �ǂݎ�����l���i�[���郁���o�ϐ�
	m_nGameScore = NULL;

	//==============================
	// GameScore.txt
	//==============================
	std::ifstream file("data\\SCORE\\GameScore.txt");

	if (file.is_open())
	{
		file >> m_nGameScore;	// ���l1��ǂݎ��
		file.close();
	}
	else
	{
		MessageBox(NULL, "GameScore.txt ���J���܂���ł���", "�G���[", MB_OK);
	}
}
