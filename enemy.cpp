//==============================
//
// �G[enemy.cpp]
// Author : Kaiti Aoto
//
//==============================
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "effect.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"
#include "game.h"
//
int CEnemy::m_nCntKill = 0;
//==================
// �R���X�g���N�^
//==================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};

	m_pModel = NULL;

	m_nLife = 0;

	m_type = TYPE_NONE;

	m_bUse = true;
	
	m_State = STATE_NONE;
	m_nCntState = 0;
}
//================
// �f�X�g���N�^
//================
CEnemy::~CEnemy()
{

}
//===========
// ��������
//===========
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CEnemy* pBullet = new CEnemy;
	if (!pBullet)
		return nullptr;

	//
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
HRESULT CEnemy::Init(void)
{
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * 1.5f, 0.0f, cosf(m_rot.y - D3DX_PI) * 1.5f);
	m_nLife = ENEMY_LIFE;
	m_bUse = true;
	m_State = STATE_STAY;
	m_nStayCounter = rand() % 120 + 60;
	m_fRandomAngle = D3DXToRadian(rand() % 360);

	//���f������
	const char* pFilename = "data/MODELS/convenience_store00.x";

	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	
	m_posHalf = D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 2), m_pos.z);

	m_pos.y += m_posHalf.y * 2.0f;

	float GeuseBase = m_nLife / 10.0f;
	//m_pGauge = CEnemyGauge::Create(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y / 1.5f), m_pos.z), GeuseBase, 5.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	//�I�u�W�F�N�g�̎�ސݒ�
	SetObjType(TYPE_ENEMY);

	return S_OK;
}
//============
// �I������
//============
void CEnemy::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	//if (m_pGauge != NULL)
	//{
	//	m_pGauge->Uninit();
	//}

	CObject::Release();
}
//============
// �X�V����
//============
void CEnemy::Update(void)
{
		if (m_bUse == true)
		{
			float rate = (float)m_nLife / (float)ENEMY_LIFE;
			rate = max(0.0f, min(rate, 1.0f));

			//m_pGauge->SetPos(D3DXVECTOR3(m_pos.x, m_pos.y + (m_size.y * 1.5f), m_pos.z));
			//m_pGauge->SetRate(rate);

			switch (m_State)
			{
			case STATE_STAY:
				//�ړ�
				Move();
				break;
			case STATE_MOVE:
				//�ړ�
				Move();
				break;
			case STATE_HIT:

				break;

			default:
				break;
			}

			//if (m_State != STATE_HIT)
			//{
			//}
			//if (m_State == STATE_STAY)
			//{
			//	m_pGauge->SetDraw(false);
			//}
			//else
			//{
			//	m_pGauge->SetDraw(true);
			//}

			//�����蔻��
			Collision();

			//����
			if (m_nLife <= 0)
			{
				m_bUse = false;
				//m_pGauge->SetDraw(false);
			}
		}
		else if (m_bUse == false)
		{//�g���Ă��Ȃ��Ȃ�
			Uninit();
		}
}
//============
// �`�揈��
//============
void CEnemy::Draw(void)
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
void CEnemy::Move(void)
{
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	D3DXVECTOR3 PlayerRot = pPlayer->GetRot();

	// �v���C���[�Ɍ������ē���
	Move_Normal();	// �ʏ�

	// �p�x�̐��K��
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	// ������ς���
	m_rot += (m_rotDest - m_rot) * 0.5f;

	// �O��̈ʒu�ۑ�
	m_posOld = m_pos;
	// �ړ�
	m_pos += m_move;

	// �n�ʔ���
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
//================
// �ʏ�ړ�����
//================
void CEnemy::Move_Normal(void)
{
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

	// �v���C���[�ւ̕����x�N�g��
	D3DXVECTOR3 toPlayer = PlayerPos - m_pos;
	// ���K��
	D3DXVec3Normalize(&toPlayer, &toPlayer);

	//�ړ���
	m_move.x = toPlayer.x * ENEMY_SPEED;
	m_move.z = toPlayer.z * ENEMY_SPEED;
	//�p�x
	float angle = atan2f(toPlayer.x, toPlayer.z);
	m_rotDest.y = angle + D3DX_PI;
}
//====================
// �G�Ƃ̓����蔻��
//====================
bool CEnemy::Collision(void)
{

	return false;
}
//================
// �_���[�W����
//================
void CEnemy::Hit(const int nDamage)
{
	m_nLife -= nDamage;

}
//=================
// ��ԑJ�ڏ���
//=================
void CEnemy::State(STATE state)
{
	m_State = state;

	switch (m_State)
	{
	case STATE_NONE:

		break;
	case STATE_HIT:
		m_nCntState = 45;
		break;
	case STATE_DEAD:
		ItemSet();
		m_bUse = false;
		break;

	default:
		break;
	}
}
//===================
// �A�C�e���Z�b�g
//===================
void CEnemy::ItemSet()
{
}