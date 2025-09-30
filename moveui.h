//==============================================================
//
// ����UI���� [ moveui.h ]
// Author: Asuma Nishio
//
//==============================================================

#ifndef _MOVEUI_H_ // ���̃}�N����`������ĂȂ�������
#define _MOVEUI_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "object2D.h"

//********************************************************
// ����UI�N���X���`
//********************************************************
class CMoveUi : public CObject2D
{
public:
	//**********************
	// �ړ��������
	//**********************
	enum MOVETYPE
	{
		MOVETYPE_NONE,
		MOVETYPE_RIGHT,
		MOVETYPE_LEFT,
		MOVETYPE_CENTER,
		MOVETYPE_DOWN,
		MOVETYPE_MAX
	};

	//**********************
	// �g��������
	//**********************
	enum SCALETYPE
	{
		SCALETYPE_NONE,
		SCALETYPE_CENTER,
		SCALETYPE_LEFTDOWN,
		SCALETYPE_MAX
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CMoveUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CMoveUi();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CMoveUi* Create(D3DXVECTOR3 pos, float fWidth, float fHeight, const char* pTexName, MOVETYPE movetype, SCALETYPE sceletype);

private:
	int m_nIdxTexture;	// �e�N�X�`���C���f�b�N�X
	MOVETYPE m_nMoveType;	// �ړ����
	SCALETYPE m_nScaleType;	// �g����
};
#endif