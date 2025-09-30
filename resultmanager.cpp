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
	m_nBulletScore = NULL;
	m_nAllScore = NULL;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_pResultScore[nCnt] = nullptr;
	}
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
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "ResultBack.jpg", false);

	// �f�[�^�̓ǂݍ���
	Load();

	// �X�R�A�𐶐����� ( �G,����,�e,���v )
	m_pResultScore[0] = CResultScore::Create(D3DXVECTOR3(1190.0f, 200.0f, 0.0f), 300.0f, 60.0f, 0, m_nGameScore);
	m_pResultScore[1] = CResultScore::Create(D3DXVECTOR3(1190.0f, 350.0f, 0.0f), 300.0f, 60.0f, 0, m_nLastTime);
	m_pResultScore[2] = CResultScore::Create(D3DXVECTOR3(1190.0f, 500.0f, 0.0f), 300.0f, 60.0f, 0, m_nBulletScore);

	// ��Ɍv�Z
	m_nAllScore = m_nGameScore + m_nLastTime + m_nBulletScore;

	// �ŏI�X�R�A���Z�b�g
	m_pResultScore[3] = CResultScore::Create(D3DXVECTOR3(1190.0f, 650.0f, 0.0f), 400.0f, 100.0f, 0, m_nAllScore);

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
			// �Ō�̃X�R�A�������o��
			m_pResultScore[3]->Save();

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
	m_nLastTime = NULL;
	m_nBulletScore = NULL;

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

	//==============================
	// TimeScore.txt
	//==============================
	std::ifstream tfile("data\\SCORE\\TimeScore.txt");

	if (tfile.is_open())
	{
		tfile >> m_nLastTime;	// ���l1��ǂݎ��
		tfile.close();
	}
	else
	{
		MessageBox(NULL, "GameScore.txt ���J���܂���ł���", "�G���[", MB_OK);
	}

	// �v�Z����
	MathScore();

	//==============================
	// BulletScore.txt
	//==============================
	std::ifstream bfile("data\\SCORE\\BulletScore.txt");

	if (bfile.is_open())
	{
		bfile >> m_nBulletScore;	// ���l1��ǂݎ��
		bfile.close();
	}
	else
	{
		MessageBox(NULL, "GameScore.txt ���J���܂���ł���", "�G���[", MB_OK);
	}

	// �v�Z����
	m_nBulletScore = m_nBulletScore * 10000;
}
//===============================
// �X�R�A�v�Z
//===============================
void CResultManager::MathScore(void)
{
	// �ǂݍ��񂾒l
	float fValue = static_cast<float>(m_nLastTime);

	// ����60�Ȃ�
	if (m_nLastTime == ADDSCORETIME)
	{
		// �ő�l���Z�b�g���� (50��)
		m_nLastTime = MAX_VALUESCORE;
		return;
	}

	// �䗦���Z�o
	float fRatio = 1.0f - (fValue /MAX_VALUESCORE);

	// �͈͓��Ŋ������Z�o
	if (fRatio < 0.0f) fRatio = 0.0f;
	if (fRatio > 1.0f) fRatio = 1.0f;

	// �X�R�A�v�Z
	int nMathscore = (int)(MAX_VALUESCORE * fRatio);

	// �Ō�ɃZ�b�g����
	m_nLastTime = nMathscore;
}
