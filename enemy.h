//==============================
//
// �G[enemy.h]
// Author : Kaiti Aoto
//
//==============================

// ��d�C���N���[�h�h�~
#ifndef _ENEMY_H_
#define _ENEMY_H_

// �C���N���[�h
#include "main.h"
#include "object.h"
#include "model.h"
//#include "gauge_enemy.h"

// 
class CShadow;

// �}�N����`
#define ENEMY_LIFE (30)
#define ENEMY_ACTIONSTATE (300)
#define ENEMY_SPEED (1.35f)

// �G�N���X
class CEnemy :public CObject
{
public:
	// ���
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_MAX
	}TYPE;
	// ���
	typedef enum
	{
		STATE_NONE = 0,
		STATE_STAY,
		STATE_MOVE,
		STATE_HIT,
		STATE_ACTION,
		STATE_DEAD,
		STATE_MAX
	}STATE;

	// �����o�֐�
	CEnemy(int nPriority = 2);
	~CEnemy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(const int nDamage);
	void MovePos(D3DXVECTOR3 move) { m_pos += move; }

	// �ݒ�
	void SetType(TYPE type) { m_type = type; };
	void SetMove(D3DXVECTOR3 move) { m_move = move; }

	// �擾
	TYPE GetType(void) { return m_type; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	

	// �ÓI�����o�֐�
	static CEnemy* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type);

private:
	void Move(void);
	void Move_Normal(void);
	

	void State(STATE state);
	bool Collision(void);
	void ItemSet();

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_posOld;		// �O��̈ʒu
	D3DXVECTOR3 m_posHalf;		// �����̍����̈ʒu
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_move;			// �ړ���
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_rotDest;		// �ڕW�̌���
	D3DXVECTOR3 m_size;			// �T�C�Y

	CModel* m_pModel;			// ���f���ւ̃|�C���^
	int m_nLife;				// ����
	int m_nCntState;			// ��ԃJ�E���^�[
	int m_nStayCounter;			// �؍ݏ�Ԃ̃J�E���g
	bool m_bUse;				// �g�p���Ă��邩
	float m_fRandomAngle;		// �����_�������̊p�x
	TYPE m_type;				// ���
	STATE m_State;				// 
	//CEnemyGauge* m_pGauge;		// �Q�[�W�ւ̃|�C���^

	//
	static int m_nCntKill;
};

#endif