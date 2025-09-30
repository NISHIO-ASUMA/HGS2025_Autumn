//==============================
//
// �G[enemy.h]
// Author:kaiti
//
//==============================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

#include "main.h"
#include "object.h"
#include "model.h"

#define MAX_DIS (300.0f)
#define GIMMICK_SPEED (8.0f)

//�G�N���X
class CGimmick :public CObject
{
public:
	//���
	typedef enum
	{
		TYPE_CAR = 0,
		TYPE_POLE,

		TYPE_MAX
	}TYPE;

	//�����o�֐�
	CGimmick(int nPriority = 3);
	~CGimmick();
	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGimmick* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type);

	void SetType(TYPE type) { m_type = type; };
	TYPE GetType(void) { return m_type; }

	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	static TYPE SetType(const char* pFileName);
private:
	void Move(void);

	//�����o�ϐ�
	D3DXVECTOR3 m_pos;					//�ʒu
	D3DXVECTOR3 m_posOld;				//�O��̈ʒu
	D3DXVECTOR3 m_posOffSet;					//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 m_rot;					//����
	D3DXVECTOR3 m_move;					//�ړ���
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_rotDest;				//�ڕW�̌���
	D3DXVECTOR3 m_size;					//�T�C�Y

	CModel* m_pModel;	//���f���ւ̃|�C���^

	TYPE m_type;			//���
	bool m_bUse;			//�g�p���Ă��邩

	float m_fMoveDis;
	float m_fMaxDis;
};

#endif