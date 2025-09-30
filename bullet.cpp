//==============================
//
// �e[bullet.cpp]
// Author : Kaiti Aoto
//
//==============================

// �C���N���[�h
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"

//==================
// �R���X�g���N�^
//==================
CBullet::CBullet(int nPriority) : CObjectX(nPriority)
{
	// �l���N���A
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};
	m_pModel = NULL;
	m_type = TYPE_NONE;
	m_user = USER_PLAYER;
	m_nLife = 0;
	m_bUse = true;
	m_bSkill = false;
}
//================
// �f�X�g���N�^
//================
CBullet::~CBullet()
{

}
//===========
// ��������
//===========
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user)
{
	// ����
	CBullet* pBullet = new CBullet();
	// ������
	if (FAILED(pBullet->Init(pos, rot, user)))
	{// NULL�`�F�b�N
		delete pBullet;
		return nullptr;
	}
	return pBullet;	// �|�C���^��Ԃ�
}
//===============
// ����������
//===============
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user)
{
	// �l����
	m_pos = pos;
	m_rot = rot;
	m_user = user;

	// �����o�ϐ���������
	m_bUse = true;		// �g�p���Ă����Ԃɂ���
	m_bSkill = false;	// �X�L���ł͂Ȃ���Ԃɂ���

	// ���f�����i�[�p�ϐ������̏�����
	const char* pFilename = "data\\MODELS\\can00.x";
	
	// �ړ��ʂ�ݒ�
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * BULLET_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * BULLET_SPEED);
	m_move.y = 10.0f;
	// ���[�U�[���v���C���[�Ȃ�
	if (m_user == USER_PLAYER)
	{
		m_nLife = BULLET_LIFE;			// ������ݒ�
	}
	// ���[�U�[���G�Ȃ�
	else if (m_user == USER_ENEMY)
	{
		m_nLife = BULLET_LIFE / 7;	// ������ݒ�
	}

	// ���f������
	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	//�T�C�Y��ݒ�


	// �I�u�W�F�N�g�̎�ސݒ�
	SetObjType(TYPE_BULLET);
	
	return S_OK;
}
//============
// �I������
//============
void CBullet::Uninit(void)
{	
	if (m_pModel != NULL)
	{// NULL�`�F�b�N
		// ���f���̏I���������j��
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	CObject::Release();
}
//============
// �X�V����
//============
void CBullet::Update(void)
{
	// �g�p���Ă����
	if (m_bUse == true)
	{
		// ���[�U�[���v���C���[�Ȃ�
		if (m_user == USER_PLAYER)
		{
			// �o�E���h�ړ�
			MoveBounce();
		}
		// ���[�U�[���G�Ȃ�
		else if(m_user == USER_ENEMY)
		{
			// �ړ�
			Move();
		}
		// �����蔻��
		Collision();

		// ���������炷
		m_nLife--;
		// �������O�ȉ��Ȃ�
		if (m_nLife <= 0)
		{
			m_bUse = false;	// �g�p���Ă��Ȃ���Ԃ�
		}
	}
	// �g���Ă��Ȃ��Ȃ�
	else if (m_bUse == false)
	{
		// �I������
		Uninit();
		return;
	}
}
//============
// �`�揈��
//============
void CBullet::Draw(void)
{
	// �g�p���Ă����
	if (m_bUse == true)
	{
		// �f�o�C�X�̎擾
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxTrans;
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ���[�U�[���v���C���[�Ȃ�
		if (m_user == USER_PLAYER)
		{
			// ���f���`��
			m_pModel->Draw();
		}
	}
}
//===========
// �ړ�����
//===========
void CBullet::Move(void)
{
	// �ʒu�X�V
	m_pos += m_move;

	// ���f���̈ʒu�X�V
	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
//==================
// �o�E���h�ړ�
//==================
void CBullet::MoveBounce(void)
{
	const float fRestitution = 0.8f;	// �����W��
	const float fMinY = 0.2f;			// �x���̍Œ�ړ���

	// �d�͉��Z
	m_move.y -= 0.7f;

	// �ʒu�X�V
	m_pos += m_move;

	// �n�ʂƂ̏Փ�
	if (m_pos.y < 0.0f)
	{
		m_pos.y = 0.0f;	// �ʒu��n�ʂƓ����ɂ���

		// Y���x�̔��]
		m_move.y *= -fRestitution;

		// �ړ��ʂ����ʏ������Ȃ�����
		if (fabs(m_move.y) < fMinY)
		{
			m_move.y = 0.0f;	// Y���̈ړ��ʂ��O��
		}
	}

	// ���f���̈ʒu�X�V
	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
//====================
// �G�Ƃ̓����蔻��
//====================
bool CBullet::Collision(void)
{
	D3DXVECTOR3 contactNormal;	// �ڐG�@��

	// ���[�U�[���v���C���[�Ȃ�
	if (m_user == USER_PLAYER)
	{

	}
	// ���[�U�[���G�Ȃ�
	else if (m_user == USER_ENEMY)
	{

	}


	return false;	 // �����Ԃ�
}
