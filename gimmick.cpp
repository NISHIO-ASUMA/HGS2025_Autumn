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
#include "characterManager.h"
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


	m_type = TYPE_TORNADO;

	m_bUse = true;

	m_fMoveDis = 0.0f;
	m_fMaxDis = MAX_DIS;

	m_pathPoints = {};
	m_currentTargetIndex = 0;
	m_dir = VECTOR3_NULL;
	m_tornadoDir = VECTOR3_NULL;
	m_nTimer = 0;
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
	CGimmick* pGimmick = new CGimmick;
	if (!pGimmick)
		return nullptr;

	pGimmick->m_pos = pos;
	pGimmick->m_rot = rot;
	pGimmick->m_type = type;

	// �ʉ߃|�C���g�̐ݒ�
	pGimmick->AddPathPoint(D3DXVECTOR3(380.0f, 0.0f, 330.0f));
	pGimmick->AddPathPoint(D3DXVECTOR3(-550.0f, 0.0f, 170.0f));
	pGimmick->AddPathPoint(D3DXVECTOR3(-430.0f, 0.0f, -390.0f));
	pGimmick->AddPathPoint(D3DXVECTOR3(-50.0f, 0.0f, -555.0f));
	pGimmick->AddPathPoint(D3DXVECTOR3(350.0f, 0.0f, -390.0f));


	if (FAILED(pGimmick->Init()))
	{
		delete pGimmick;
		return nullptr;
	}

	return pGimmick;
}
//===============
// ����������
//===============
HRESULT CGimmick::Init()
{
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * GIMMICK_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * GIMMICK_SPEED);

	m_bUse = true;
	m_posOffSet = m_pos;
	if (m_type == TYPE_TORNADO)
	{
		m_pModel = CModel::Create("data/MODELS/tornado00.x", m_pos, m_rot);
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
//=============================================================================
// �ʉ߃|�C���g�ǉ�����
//=============================================================================
void CGimmick::AddPathPoint(const D3DXVECTOR3& point)
{
	m_pathPoints.push_back(point);
}
//===========
// �ړ�����
//===========
void CGimmick::Move(void)
{
	// �v���C���[���擾
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	D3DXVECTOR3 playerPos = pPlayer->GetPos();

	D3DXVECTOR3 disPos = playerPos - GetPos();
	float distance = D3DXVec3Length(&disPos);

	const float kTriggerDistance = 150.0f; // ��������

	if (distance < kTriggerDistance)
	{
		m_nTimer++;

		if (m_nTimer >= 60)
		{// �����_���[�W
			pPlayer->Hit(1);
		}
	}
	else
	{
		// �^�C�}�[���Z�b�g
		m_nTimer = 0;
	}

	// �����̕����Ƀv���C���[���Ђ�����
	m_tornadoDir = GetPos() - playerPos;
	float distTornade = D3DXVec3Length(&m_tornadoDir);


	m_pos += m_move;

	m_rot.y += 0.08f;

	if (m_rot.y > D3DX_PI * 2.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	if (m_pathPoints.empty() || m_currentTargetIndex >= (int)m_pathPoints.size())
	{
		return;
	}

	D3DXVECTOR3 currentPos = m_pos;
	D3DXVECTOR3 targetPos = m_pathPoints[m_currentTargetIndex];

	// �^�[�Q�b�g�����x�N�g��
	m_dir = targetPos - currentPos;
	float dist = D3DXVec3Length(&m_dir);

	if (dist < 100.0f)  // ������x�߂Â����玟�̃|�C���g��
	{
		if (m_currentTargetIndex >= 4)
		{
			m_currentTargetIndex = 0;
		}
		else
		{
			m_currentTargetIndex++;
		}
		return;
	}

	// ���K��
	D3DXVec3Normalize(&m_dir, &m_dir);
	D3DXVec3Normalize(&m_tornadoDir, &m_tornadoDir);

	// �ړ��ʂ̐ݒ�
	m_move = D3DXVECTOR3(m_dir.x * GIMMICK_SPEED, 0.0f, m_dir.z * GIMMICK_SPEED);
	D3DXVECTOR3 moveDis = D3DXVECTOR3(m_tornadoDir.x * 2.0f, 0.0f, m_tornadoDir.z * 2.0f);

	playerPos += moveDis;

	pPlayer->SetPos(playerPos);
	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
////=====================
//// ���f�������Ƃ̎��
////=====================
//CGimmick::TYPE CGimmick::SetType(const char* pFileName)
//{
//	CGimmick::TYPE type = CGimmick::TYPE_CAR;
//
//	if (strcmp(pFileName, "data\\MODEL\\bluecar.x") == 0)
//	{
//		type = CGimmick::TYPE_CAR;
//	}
//	//else if (strcmp(pFileName, "data\\MODEL\\vending_machine03.x") == 0)
//	//{
//	//}
//	return type;
//}
