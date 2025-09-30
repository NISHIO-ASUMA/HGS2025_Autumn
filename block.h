//=============================================================================
//
// �u���b�N���� [block.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "objectX.h"
#include "collisionUtils.h"

//*****************************************************************************
// �u���b�N�N���X
//*****************************************************************************
class CBlock : public CObjectX
{
public:
	CBlock(int nPriority = 3);
	virtual ~CBlock() = default;

	// �u���b�N�̎��
	typedef enum
	{
		TYPE_CONVENI = 0,
		TYPE_MAX
	}TYPE;

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);	// �u���b�N�̐���
	void InitFactry(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �R���C�_�[�����i�f�t�H���g�̓{�b�N�X�j
	virtual CBoxCollider* CreateCollider(void)
	{
		return new CBoxCollider(GetModelSize());
	}

	//*****************************************************************************
	// setter�֐�
	//*****************************************************************************
	void SetType(TYPE type) { m_Type = type; }											// �^�C�v�̐ݒ�

	//*****************************************************************************
	// getter�֐�
	//*****************************************************************************
	TYPE GetType(void) const { return m_Type; }											// �^�C�v�̎擾
	D3DXMATRIX GetWorldMatrix(void);
	CBoxCollider* GetCollider(void) { return m_pCollider; }

private:
	TYPE m_Type;				// ���
	CBoxCollider* m_pCollider;	// �{�b�N�X�R���C�_�[
	static std::unordered_map<std::string, std::function<CBlock* ()>> blockFactory;
};

//*****************************************************************************
// �R���r�j�u���b�N�N���X
//*****************************************************************************
class CConveniBlock : public CBlock
{
public:
	CConveniBlock();
	~CConveniBlock();

	void Update(void);

private:

	int m_nCntSpan;
};


#endif