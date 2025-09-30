////=======================
//
// �^�C�}�[[timer.cpp]
// Author:kaiti
//
//=======================
#include "bullet_counter.h"
#include "renderer.h"
#include "manager.h"
//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "score.h"
#include "manager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

//**********************
// �ÓI�����o�ϐ��錾
//**********************
int CBulletCnt::m_nScore = NULL;	// ���X�R�A�J�E���g�p
int CBulletCnt::m_nDecCount = NULL; // ���Z�X�R�A�J�E���g�p

//==========================================
// �R���X�g���N�^
//==========================================
CBulletCnt::CBulletCnt(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_nScore = NULL;
	m_nIdxTexture = NULL;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;

	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		m_apNumber[nCnt] = nullptr;
	}
}
//==========================================
// �f�X�g���N�^
//==========================================
CBulletCnt::~CBulletCnt()
{
	// ����
}
//==========================================
// ��������
//==========================================
CBulletCnt* CBulletCnt::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	// �X�R�A�C���X�^���X����
	CBulletCnt* pScore = new CBulletCnt;

	// nullptr��������
	if (pScore == nullptr) return nullptr;

	// ���W,�T�C�Y�ݒ�
	pScore->m_pos = pos;
	pScore->m_fWidth = fWidth;
	pScore->m_fHeight = fHeight;

	// ���������s��
	if (FAILED(pScore->Init()))
	{
		// nullptr��Ԃ�
		return nullptr;
	}

	// �X�R�A�|�C���^��Ԃ�
	return pScore;
}
//==========================================
// ����������
//==========================================
HRESULT CBulletCnt::Init(void)
{
	// �����v�Z
	float fTexPos = m_fWidth / NUM_SCORE;

	// ������
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// �C���X�^���X����
		m_apNumber[nCnt] = new CNumber;

		// ����������
		m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x - (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fHeight);

		// �i���o�[�ϐ��̃T�C�Y
		m_apNumber[nCnt]->SetSize(fTexPos, m_fHeight);

		// ���W�ݒ�
		m_apNumber[nCnt]->SetPos(m_pos);

		// �e�N�X�`���Z�b�g
		m_apNumber[nCnt]->SetTexture("number.png");
	}

	// ���������ʂ�Ԃ�
	return S_OK;
}
//==========================================
// �I������
//==========================================
void CBulletCnt::Uninit(void)
{
	// �g�������j��
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// nullptr�`�F�b�N
		if (m_apNumber[nCnt] != nullptr)
		{
			// �I��
			m_apNumber[nCnt]->Uninit();

			// �j��
			delete m_apNumber[nCnt];

			// �|�C���^������
			m_apNumber[nCnt] = nullptr;
		}
	}

	// ���g�̔j��
	CObject::Release();
}
//==========================================
// �X�V����
//==========================================
void CBulletCnt::Update(void)
{
	// �X�R�A�i�[
	int nScore = m_nScore;

	// ������
	for (int nCntScore = 0; nCntScore < NUM_SCORE; nCntScore++) // �E���珈��
	{
		// �������Ƃɕ�������l���v�Z
		int nDigit = nScore % NUM_DIGIT;

		nScore /= NUM_DIGIT;

		// �i���o�[�X�V
		m_apNumber[nCntScore]->Update();

		// ���X�V
		m_apNumber[nCntScore]->SetDigit(nDigit);
	}
}
//==========================================
// �`�揈��
//==========================================
void CBulletCnt::Draw(void)
{
	// �g���Ă��錅�����̕`��
	for (int nCnt = 0; nCnt < NUM_SCORE; nCnt++)
	{
		// �i���o�[�`��
		m_apNumber[nCnt]->Draw();
	}
}
//==========================================
// �X�R�A���Z����
//==========================================
void CBulletCnt::AddScore(int nValue)
{
	// �X�R�A���Z
	m_nScore += nValue;
}
//==========================================
// �X�R�A������
//==========================================
void CBulletCnt::DeleteScore()
{
	m_nScore = 0;
}
//==========================================
// �X�R�A���Z����
//==========================================
void CBulletCnt::DecScore(void)
{
	// ���Z�񐔂��L�^
	m_nDecCount++;
}
//==========================================
// �X�R�A�����o������
//==========================================
void CBulletCnt::SaveScore(void)
{
	// �J���t�@�C�����`
	std::ofstream File("data\\SCORE\\GameScore.txt");

	// ��O����
	if (!File.is_open())
	{
		MessageBox(NULL, "�ۑ��t�@�C�����J���܂���ł���", "�G���[", MB_OK);

		return;
	}

	// ���l���o�͂���
	File << m_nScore;

	// �t�@�C�������
	File.close();
}
//==========================================
// �e�N�X�`���ݒ�
//==========================================
void CBulletCnt::SetTexture(void)
{
	// �e�N�X�`���|�C���^�擾
	CTexture* pTexture = CManager::GetTexture();

	// �e�N�X�`�����蓖��
	m_nIdxTexture = pTexture->RegisterDynamic("data\\TEXTURE\\number.png");
}