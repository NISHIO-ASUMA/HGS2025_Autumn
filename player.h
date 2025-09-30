//=============================================================================
//
// �v���C���[���� [player.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _PLAYER_H_// ���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_// 2�d�C���N���[�h�h�~�̃}�N����`

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "model.h"
#include "motion.h"
#include "block.h"
#include "shadowS.h"
#include "state.h"
#include "manager.h"
#include "gaugePlayer.h"
#include "bullet.h"

// �O���錾
class CPlayer_StandState;
class CPlayer_MoveState;
class CPlayer_JumpState;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTS		(32)					// �ő�p�[�c��
#define PLAYER_SPEED	(5.0f)					// �ړ��X�s�[�h
#define MAX_JUMP_POWER	(330.3f)				// �W�����v����
#define CAPSULE_RADIUS (18.5f)					// �J�v�Z���R���C�_�[�̔��a
#define CAPSULE_HEIGHT (55z.5f)					// �J�v�Z���R���C�_�[�̍���
#define PLAYER_LIFE (10)

// ���̓f�[�^�\����
struct InputData
{
	D3DXVECTOR3 moveDir;      // �ړ��x�N�g��
	bool jump;                // �W�����v����
};

//*****************************************************************************
// �v���C���[�N���X
//*****************************************************************************
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = 2);
	~CPlayer();

	// �v���C���[���[�V�����̎��
	typedef enum
	{
		NEUTRAL = 0,		// �ҋ@
		MOVE,				// �ړ�
		JUMP,				// �W�����v
		MAX
	}PLAYER_MOTION;

	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//*****************************************************************************
	// setter�֐�
	//*****************************************************************************
	void SetMotion(int type, int nBlendFrame);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetLife(int nLife) { m_nLife = nLife; }

	//*****************************************************************************
	// getter�֐�
	//*****************************************************************************
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetMove(void) const { return m_move; }
	CCapsuleCollider* GetCollider(void) { return m_pCollider; }
	InputData GatherInput(void);
	int GetLife(void) { return m_nLife; }

private:
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXVECTOR3 m_colliderPos;			// �J�v�Z�����S
	D3DXVECTOR3 m_rot;					// ����
	D3DXVECTOR3 m_rotDest;				// ����
	D3DXVECTOR3 m_move;					// �ړ���
	D3DXVECTOR3 m_targetMove;			// �ڕW���x
	D3DXVECTOR3 m_currentMove;			// ���ۂ̈ړ����x
	D3DXVECTOR3 m_size;					// �T�C�Y
	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	CModel* m_apModel[MAX_PARTS];		// ���f��(�p�[�c)�ւ̃|�C���^
	CShadowS* m_pShadowS;				// �X�e���V���V���h�E�ւ̃|�C���^
	CMotion* m_pMotion;					// ���[�V�����ւ̃|�C���^
	int m_currentMotion;				// ���݂̃��[�V����
	int m_nNumModel;					// ���f��(�p�[�c)�̑���
	int m_nLife;						// ���C�t
	CCapsuleCollider* m_pCollider;		// �J�v�Z���R���C�_�[
	CHpGauge* m_pHpGauge;				// �g�o�Q�[�W�ւ̃|�C���^

	// �X�e�[�g���Ǘ�����N���X�̃C���X�^���X
	StateMachine<CPlayer> m_stateMachine;
};


//*****************************************************************************
// �v���C���[�̑ҋ@���
//*****************************************************************************
class CPlayer_StandState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* pPlayer)override
	{
		// �ҋ@���[�V����
		pPlayer->SetMotion(CPlayer::NEUTRAL, 10);
	}

	void OnUpdate(CPlayer* pPlayer)override
	{
		// ���͂��擾
		InputData input = pPlayer->GatherInput();

		CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
		CInputJoypad* pJoypad = CManager::GetJoyPad();				// �W���C�p�b�h�̎擾

		// ---------------------------
		// �e����
		// ---------------------------
		if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTriggerR2())
		{
			CBullet::Create(pPlayer->GetPos(), pPlayer->GetRot(), CBullet::USER_PLAYER);
		}
		if (pKeyboard->GetTrigger(DIK_1) || pJoypad->GetTriggerR2())
		{
			int nLife = pPlayer->GetLife();
			nLife--;
			pPlayer->SetLife(nLife);
		}

		// �ړ����͂�����ꍇ
		if (input.moveDir.x != 0.0f || input.moveDir.z != 0.0f)
		{
			// �ړ��X�e�[�g�Ɉڍs
			m_pMachine->ChangeState<CPlayer_MoveState>();
		}
	}

	void OnExit(CPlayer* /*pPlayer*/)override
	{

	}

private:

};

//*****************************************************************************
// �v���C���[�̈ړ����
//*****************************************************************************
class CPlayer_MoveState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* pPlayer)override
	{
		// �ړ����[�V����
		pPlayer->SetMotion(CPlayer::MOVE, 10);
	}

	void OnUpdate(CPlayer* pPlayer)override
	{
		// ���͂��擾
		InputData input = pPlayer->GatherInput();

		CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	// �L�[�{�[�h�̎擾
		CInputJoypad* pJoypad = CManager::GetJoyPad();				// �W���C�p�b�h�̎擾

		// ---------------------------
		// �e����
		// ---------------------------
		if (pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTriggerR2())
		{
			CBullet::Create(pPlayer->GetPos(), pPlayer->GetRot(), CBullet::USER_PLAYER);
		}

#ifdef _DEBUG
		// HP�����炷
		if (pKeyboard->GetTrigger(DIK_1))
		{
			int nLife = pPlayer->GetLife();
			nLife--;
			pPlayer->SetLife(nLife);
		}
#endif

		pPlayer->SetMove(input.moveDir * PLAYER_SPEED);

		if (input.moveDir.x == 0.0f || input.moveDir.z == 0.0f)
		{
			// �ړ����͂�������Αҋ@�X�e�[�g�ɖ߂�
			m_pMachine->ChangeState<CPlayer_StandState>();
		}
	}

	void OnExit(CPlayer* /*pPlayer*/)override
	{

	}

private:

};

//*****************************************************************************
// �v���C���[�̃W�����v���
//*****************************************************************************
class CPlayer_JumpState :public StateBase<CPlayer>
{
public:

	void OnStart(CPlayer* pPlayer)override
	{
		// �W�����v���[�V����
		pPlayer->SetMotion(CPlayer::JUMP, 10);
	}

	void OnUpdate(CPlayer* pPlayer)override
	{
		// ���͂��擾
		InputData input = pPlayer->GatherInput();


	}

	void OnExit(CPlayer* /*pPlayer*/)override
	{

	}

private:

};

#endif
