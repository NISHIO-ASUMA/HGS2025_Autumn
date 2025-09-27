//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "block.h"
#include "manager.h"

// ���O��Ԃ̎g�p
using namespace std;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlock::CBlock(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_pCollider = nullptr;
}
//=============================================================================
// ��������
//=============================================================================
CBlock* CBlock::Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CBlock* pBlock = new CBlock();

	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->SetSize(size);
	pBlock->SetPath(pFilepath);// ���f���p�X

	// ����������
	pBlock->Init();

	// �{�b�N�X�R���C�_�[�̐���
	pBlock->m_pCollider = new CBoxCollider(size);

	return pBlock;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CBlock::Init(void)
{
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CBlock::Uninit(void)
{
	// �R���C�_�[�̔j��
	if (m_pCollider != nullptr)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}

	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBlock::Update(void)
{
	if (m_pCollider)
	{
		// �R���C�_�[�̍X�V
		m_pCollider->UpdateTransform(GetPos(), GetRot(), GetSize());
	}

	//// �v���C���[�擾
	//CPlayer* pPlayer = CGame::GetPlayer();

	//if (pPlayer && pPlayer->GetCollider())
	//{
	//	// �J�v�Z����OBB�̓����蔻��
	//	if (CCollision::CheckCapsuleOBBCollision(pPlayer->GetCollider(),m_pCollider))
	//	{


	//	}
	//}

	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}
//=============================================================================
// �`�揈��
//=============================================================================
void CBlock::Draw(void)
{
	// �I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}
//=============================================================================
// ���[���h�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX CBlock::GetWorldMatrix(void)
{
	D3DXMATRIX matScale, matRot, matTrans;

	// �X�P�[���s��
	D3DXVECTOR3 scale = GetSize(); // �g�嗦
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// ��]�s��
	D3DXVECTOR3 rot = GetRot(); // ���W�A���p
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// ���s�ړ��s��
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	// �����FS * R * T
	D3DXMATRIX world = matScale * matRot * matTrans;

	return world;
}
