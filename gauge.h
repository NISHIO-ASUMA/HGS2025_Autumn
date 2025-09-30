//==============================
//
//  �Q�[�W����[Gauge.h]
//  Author : Kaiti Aoto
//
//==============================

// ��d�C���N���[�h�h�~
#ifndef _GAUGE_H_
#define _GAUGE_H_

// �C���N���[�h
#include"main.h"
#include "object.h"

// �}�N����`
#define GAUGE_X (40)
#define GAUGE_Y (20)

// �Q�[�W�N���X
class CGauge
{
public:

	// �����o�֐�
	CGauge();
	~CGauge();
	HRESULT Init(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ݒ�
	void SetBase(float base) { m_Base = base; }
	void SetRate(float rate) { m_rate = rate; }
	void SetColor(D3DXCOLOR col) { m_col = col; }
	void Set();

	// �ÓI�����o�֐�
	static CGauge* Create(D3DXVECTOR3 pos, float base, float fHeight, D3DXCOLOR col);

private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// �o�b�t�@
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXCOLOR m_col;					// �F
	float m_Base;						// �
	float m_fHeight;					// ����
	float m_rate;						// ���[�g

	static int m_nIdxTexture;			// �e�N�X�`���̃C���f�b�N�X�ԍ�

};
#endif