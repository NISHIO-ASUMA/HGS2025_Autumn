//=====================================
//
// �^�C������ [ time.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TIME_H_ // ���̃}�N����`������ĂȂ�������
#define _TIME_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

//**********************
// �C���N���[�h�t�@�C��
//**********************
#include "object.h"

//**********************
// �O���錾
//**********************
class CNumber;

//**********************************
// �^�C���N���X���`
//**********************************
class CTime : public CObject
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CTime(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CTime();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Save(void);

	// �Q�b�^�[
	int GetAllTime(void) { return m_nAllTime; }
	int GetcurrentTime(void) { return m_nSecond; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	// �ÓI�����o�֐�
	static CTime* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:
	// �����o�֐�
	void Second(void);
	void Minute(void);

	static constexpr int DIGIT_TIME = 2;		// ����
	static constexpr int NUMTIME = 60;			// �ő�^�C�}�[
	static constexpr int CARVETIME = 60;		// �J�E���g���
	static constexpr int DIVIDE = 10;			// �������̒l
	static constexpr int REDTIMEZONE = 10;		// �Ԃ��_�ł����鎞��
	static inline constexpr float VALUE_WIDTH = 150.0f; // �����̊Ԋu

	int m_nAllTime;		// �S�̂̎���
	int m_nSecond;		// ���݂̎��� ( �b )
	int m_nMinute;		// ���݂̎��� (�� )
	int m_nDecTime;		// ���������ԗ�
	int m_nCount;		// ���ԃJ�E���^�[
	float m_fHeight, m_fWidth;	// ����,����

	CNumber* m_pNumberMinute[DIGIT_TIME];	// ��
	CNumber* m_pNumberSecond[DIGIT_TIME];	// �b
	D3DXVECTOR3 m_pos;		// ���W
};
#endif