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
#define CAPSULE_HEIGHT (55.5f)					// �J�v�Z���R���C�_�[�̍���

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
		BESIDEMOVE,			// ���ړ�
		JUMP,				// �W�����v
		HOLD_NEUTRAL,		// �͂ݑҋ@
		HOLD_MOVE,			// �݈͂ړ�
		HOLD_BESIDEMOVE,	// �͂݉��ړ�
		HOLD_JUMP,			// �͂݃W�����v
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

	//*****************************************************************************
	// getter�֐�
	//*****************************************************************************
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; };
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetMove(void) const { return m_move; }
	InputData GatherInput(void);

private:
	D3DXVECTOR3 m_pos;					// �ʒu
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
