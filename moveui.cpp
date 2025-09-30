//==========================================================
//
// �J�����A�j���[�V�������ɓK�p����UI���� [ moveui.cpp ]
// Author: Asuma Nishio
//
//==========================================================

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "moveui.h"
#include "manager.h"
#include "texture.h"

//**********************
// ���O���
//**********************
namespace MOVEUIINFO
{
	constexpr float MOVESPEED_X = 12.0f;
	constexpr float MOVESPEED_Y = 15.0f;
};

//=================================
// �I�[�o�[���[�h�R���X�g���N�^
//=================================
CMoveUi::CMoveUi(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_nIdxTexture = NULL;
	m_nMoveType = MOVETYPE_NONE;
	m_nScaleType = SCALETYPE_NONE;
}
//=================================
// �f�X�g���N�^
//=================================
CMoveUi::~CMoveUi()
{
	// ����
}
//=================================
// ������������
//=================================
CMoveUi* CMoveUi::Create(D3DXVECTOR3 pos, float fWidth,float fHeight, const char * pTexName,MOVETYPE movetype,SCALETYPE sceletype)
{
	// �C���X�^���X����
	CMoveUi* pMoveUi = new CMoveUi;
	if (pMoveUi == nullptr) return nullptr;

	// ���������s��
	if (FAILED(pMoveUi->Init()))
	{
		return nullptr;
	}

	// 2D�I�u�W�F�N�g�ݒ�
	pMoveUi->SetPos(pos);
	pMoveUi->SetSize(fWidth, fHeight);
	pMoveUi->SetTexture(pTexName);
	pMoveUi->m_nMoveType = movetype;
	pMoveUi->m_nScaleType = sceletype;
	pMoveUi->SetAnchor(CObject2D::ANCHORTYPE_CENTER);
	pMoveUi->SetDrawType(0);

	// �������ꂽ�|�C���^
	return pMoveUi;
}
//=================================
// ����������
//=================================
HRESULT CMoveUi::Init(void)
{
	// �e�N���X�̏���������
	CObject2D::Init();

	// ���������ʂ�Ԃ�
	return S_OK;
}
//=================================
// �I������
//=================================
void CMoveUi::Uninit(void)
{
	// �e�N���X�̏I������
	CObject2D::Uninit();
}
//=================================
// �X�V����
//=================================
void CMoveUi::Update(void)
{
	// ���ݍ��W���擾
	D3DXVECTOR3 NowPos = GetPos();

	// ��ʒ������W
	float fCenterPos = SCREEN_WIDTH * 0.5f;
	float fCenterPosHeight = SCREEN_HEIGHT * 0.5f;

	switch (m_nMoveType)
	{
	case MOVETYPE_NONE:
		break;

	case MOVETYPE_RIGHT: // �E���璆����

		if (NowPos.x > fCenterPos)
		{
			NowPos.x -= MOVEUIINFO::MOVESPEED_X;

			if (NowPos.x <= fCenterPos)
			{
				NowPos.x = fCenterPos;
				m_nMoveType = MOVETYPE_CENTER; // ���B�����璆���Œ�
			}
			SetPos(NowPos);
		}

		break;

	case MOVETYPE_LEFT:   // �����璆����

		if (NowPos.x < fCenterPos)
		{
			NowPos.x += MOVEUIINFO::MOVESPEED_X;

			if (NowPos.x >= fCenterPos)
			{
				NowPos.x = fCenterPos;
				m_nMoveType = MOVETYPE_CENTER; // ���B�����璆���Œ�
			}

			SetPos(NowPos);
		}
		break;

	case MOVETYPE_CENTER: // �����Ŏ~�܂�

		NowPos.x = fCenterPos;
		SetPos(NowPos);

		break;

	case MOVETYPE_DOWN:   // �ォ�牺

		if (NowPos.y < fCenterPosHeight)
		{
			NowPos.y += MOVEUIINFO::MOVESPEED_Y;

			if (NowPos.y >= fCenterPosHeight)
			{
				NowPos.x = fCenterPosHeight;
				m_nMoveType = MOVETYPE_CENTER; // ���B�����璆���Œ�
			}

			SetPos(NowPos);
		}
		break;

	}

	switch (m_nScaleType)
	{
	case CMoveUi::SCALETYPE_NONE:
		break;

	case CMoveUi::SCALETYPE_CENTER: // ��������
	{
		// �ő啝
		const float fMaxWidth = 150.0f;		// ����

		// �g��X�s�[�h
		const float fAddWidth = 3.0f;		// �t���[�����̉��Z��

		// ���݂̕�
		float fWidth = GetWidth();

		if (fWidth < fMaxWidth)
		{
			fWidth += fAddWidth;

			if (fWidth >= fMaxWidth)
			{
				fWidth = fMaxWidth;
				m_nScaleType = SCALETYPE_LEFTDOWN; //���Ɉړ�����
			}

			SetWidth(fWidth);
		}
	}
	break;

	case SCALETYPE_LEFTDOWN:
	{
		// �ڕW���W
		D3DXVECTOR3 DestPos = { 135.0f, 600.0f, 0.0f };

		const float fDestHeight = 40.0f;	// ����

		// �ړ��X�s�[�h
		const float fLerpSpeed = 0.05f;

		// ���ݒl�擾
		float fHeight = GetHeight();

		// ���`���
		NowPos.x += (DestPos.x - NowPos.x) * fLerpSpeed;
		NowPos.y += (DestPos.y - NowPos.y) * fLerpSpeed;

		// ���������X�ɏk��
		fHeight += (fDestHeight - fHeight) * fLerpSpeed;

		SetPos(NowPos);
		SetHeight(fHeight);
		SetWidth(140.0f);

		// �I������
		if (fabs(DestPos.x - NowPos.x) < 1.0f &&
			fabs(DestPos.y - NowPos.y) < 1.0f &&
			fabs(fDestHeight - fHeight) < 1.0f)
		{
			SetPos(DestPos);		// �I�u�W�F�N�g�Z�b�g
			SetHeight(fDestHeight);	// �I�u�W�F�N�g�Z�b�g
			m_nScaleType = SCALETYPE_NONE;
		}	
	}
		break;

	default:
		break;
	}

	// �e�N���X�X�V����
	CObject2D::Update();
}
//=================================
// �`�揈��
//=================================
void CMoveUi::Draw(void)
{
	// �I�u�W�F�N�g�̕`��
	CObject2D::Draw();
}
