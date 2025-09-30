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
#include "player.h"
#include "characterManager.h"
#include "particle.h"

// ���O��Ԃ̎g�p
using namespace std;

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
std::unordered_map<std::string, std::function<CBlock* ()>> CBlock::blockFactory;

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
	CBlock* pBlock = nullptr;

	if (blockFactory.empty())
	{
		// �t�@�N�g���[
		pBlock->InitFactry();
	}

	auto it = blockFactory.find(pFilepath);
	if (it != blockFactory.end())
	{
		pBlock = it->second(); // �����_�Ŕh���N���X�𐶐�
	}
	else
	{
		pBlock = new CBlock(); // �f�t�H���g
	}

	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->SetSize(size);
	pBlock->SetPath(pFilepath);// ���f���p�X

	// ����������
	pBlock->Init();

	// �{�b�N�X�R���C�_�[�̐���
	pBlock->m_pCollider = pBlock->CreateCollider();

	return pBlock;
}
//=============================================================================
// �t�@�N�g���[����
//=============================================================================
void CBlock::InitFactry(void)
{
	blockFactory["data/MODELS/convenience_store00.x"] = []() { return new CConveniBlock(); };
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

	// �v���C���[�̎擾
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	// �J�v�Z����OBB�̓����蔻��
	if (CCollision::CheckCapsuleOBBCollision(
		(CCapsuleCollider*)pPlayer->GetCollider(),
		(CBoxCollider*)m_pCollider))
	{
		// �J�v�Z���̉����߂�����
		CCollision::PushCapsuleOutOfOBB((CCapsuleCollider*)pPlayer->GetCollider(),
			(CBoxCollider*)m_pCollider);
	}

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


//=============================================================================
// �R���r�j�u���b�N�̃R���X�g���N�^
//=============================================================================
CConveniBlock::CConveniBlock()
{
	// �^�C�v��ݒ�
	SetType(TYPE_CONVENI);
}
//=============================================================================
// �R���r�j�u���b�N�̃f�X�g���N�^
//=============================================================================
CConveniBlock::~CConveniBlock()
{
	// �Ȃ�
}
//=============================================================================
// �R���r�j�u���b�N�̍X�V����
//=============================================================================
void CConveniBlock::Update(void)
{
	CBlock::Update();

	//// �I�t�Z�b�g
	//D3DXVECTOR3 localOffset(0.0f, 60.0f, 0.0f); // �����̐�[�i���[�J���j
	//D3DXVECTOR3 worldOffset;

	//// �u���b�N�̃��[���h�}�g���b�N�X���擾
	//D3DXMATRIX worldMtx = GetWorldMatrix();

	//D3DXVec3TransformCoord(&worldOffset, &localOffset, &worldMtx);

	//// �p�[�e�B�N������
	//CParticle::Create<CFireParticle>(VECTOR3_NULL, worldOffset, D3DXCOLOR(0.8f, 0.5f, 0.1f, 0.8f), 8, 1);
	//CParticle::Create<CFireParticle>(VECTOR3_NULL, worldOffset, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.8f), 15, 1);
}
