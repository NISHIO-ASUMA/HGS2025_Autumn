//==============================
//
// �G[enemy.cpp]
// Author:kaiti
//
//==============================
#include "gimmick.h"
#include "renderer.h"
#include "manager.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "enemy.h"
//==================
// �R���X�g���N�^
//==================
CGimmick::CGimmick(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pModel = NULL;


	m_type = TYPE_CAR;

	m_bUse = true;

	m_fMoveDis = 0.0f;
	m_fMaxDis = MAX_DIS;
}
//================
// �f�X�g���N�^
//================
CGimmick::~CGimmick()
{

}
//===========
// ��������
//===========
CGimmick* CGimmick::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CGimmick* pBullet = new CGimmick;
	if (!pBullet)
		return nullptr;

	pBullet->m_pos = pos;
	pBullet->m_rot = rot;
	pBullet->m_type = type;

	if (FAILED(pBullet->Init()))
	{
		delete pBullet;
		return nullptr;
	}

	return pBullet;
}
//===============
// ����������
//===============
HRESULT CGimmick::Init()
{
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * GIMMICK_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * GIMMICK_SPEED);

	m_bUse = true;
	m_posOffSet = m_pos;
	if (m_type == TYPE_CAR)
	{
		m_pModel = CModel::Create("data\\MODEL\\car000.x", m_pos, m_rot);
	}

	return S_OK;
}
//============
// �I������
//============
void CGimmick::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}
	CObject::Release();
}
//============
// �X�V����
//============
void CGimmick::Update(void)
{
	if (m_bUse == true)
	{
		//�ړ�
		Move();
	}
	else if (m_bUse == false)
	{//�g���Ă��Ȃ��Ȃ�
		Uninit();
	}
}
//============
// �`�揈��
//============
void CGimmick::Draw(void)
{
	if (m_bUse == true)
	{
		//�f�o�C�X�̎擾
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxRot, mtxTrans;
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//���f���`��
		m_pModel->Draw();
	}
}
//===========
// �ړ�����
//===========
void CGimmick::Move(void)
{
	m_posOld = m_pos;

	m_pos += m_move;

	m_fMoveDis += D3DXVec3Length(&m_move);

	if (m_fMoveDis >= m_fMaxDis)
	{
		m_rot.y += D3DX_PI;
		if (m_rot.y > D3DX_PI * 2.0f)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}

		m_fMoveDis = 0.0f;
		m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * GIMMICK_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * GIMMICK_SPEED);
	}

	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);

}
//=====================
// ���f�������Ƃ̎��
//=====================
CGimmick::TYPE CGimmick::SetType(const char* pFileName)
{
	CGimmick::TYPE type = CGimmick::TYPE_CAR;

	if (strcmp(pFileName, "data\\MODEL\\bluecar.x") == 0)
	{
		type = CGimmick::TYPE_CAR;
	}
	//else if (strcmp(pFileName, "data\\MODEL\\vending_machine03.x") == 0)
	//{
	//}
	return type;
}
