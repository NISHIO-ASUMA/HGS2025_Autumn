//==============================
//
//  HP�Q�[�W����[gauge_hp.h]
//  Author : Kaiti Aoto
//
//==============================

// ��d�C���N���[�h�h�~
#ifndef _GAUGEPLAYER_H_
#define _GAUGEPLAYER_H_

// �C���N���[�h
#include"main.h"
#include "gauge.h"

// HP�Q�[�W�N���X
class CHpGauge :CObject
{
public:
	// �����o�֐�
	CHpGauge(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CHpGauge();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CHpGauge* Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXCOLOR m_col;	// �F
	float m_Base;		// �
	float m_fHeight;	// ����
	float m_rate;		// ���[�g

	CGauge* m_pGauge;	// �Q�[�W�ւ̃|�C���^
};
#endif
