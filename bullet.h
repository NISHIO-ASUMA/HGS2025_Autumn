//==============================
//
// �e[bullet.h]
// Author : Kaiti Aoto
//
//==============================

// ��d�C���N���[�h�h�~
#ifndef _BULET_H_
#define _BULET_H_

// �C���N���[�h
#include "main.h"
#include "object.h"
#include "model.h"
#include "effect.h"

// �}�N����`
#define BULET_SIZE (30)
#define BULLET_LIFE (200)
#define BULLET_ATTACK (10)
#define BULLET_SPEED (7.5f)

// �e�N���X
class CBullet:public CObjectX
{
public:
	// ���[�U�[
	typedef enum
	{
		USER_PLAYER = 0,
		USER_ENEMY,
		USER_MAX
	}USER;
	// ���
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_MAX
	}TYPE;

	// �����o�֐�
	CBullet(int nPriority = 3);
	~CBullet();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �擾
	int GetUser(void) { return m_user; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	TYPE GetType(void) { return m_type; }
	bool GetUse(void) { return m_bUse; }
	bool GetSkill(void) { return m_bSkill; }
	// �ݒ�
	void SetUser(USER user) { m_user = user; };
	void SetUse(bool bUse) { m_bUse = bUse; }
	void SetSkill(bool bSkill) { m_bSkill = bSkill; }

	// �ÓI�����o�֐�
	static CBullet* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user);

private:
	void Move(void);
	void MoveBounce(void);
	bool Collision(void);

	// �����o�ϐ�
	D3DXVECTOR3 m_pos;					// �ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 m_rot;					// ����
	D3DXVECTOR3 m_move;					// �ړ���
	D3DXMATRIX m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_rotDest;				// �ڕW�̌���
	D3DXVECTOR3 m_size;					// �T�C�Y

	CModel* m_pModel;					// ���f���ւ̃|�C���^
	int m_nLife;						// ����
	TYPE m_type;						// ���
	USER m_user;						// ���[�U�[
	bool m_bUse;						// �g�p���Ă��邩
	bool m_bSkill;						// �X�L�����ǂ���
};

#endif