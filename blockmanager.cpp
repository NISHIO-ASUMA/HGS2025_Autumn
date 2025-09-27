//=============================================================================
//
// �u���b�N�}�l�[�W���[���� [blockmanager.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "blockmanager.h"
#include "json.hpp"
#include "manager.h"

// JSON�̎g�p
using json = nlohmann::json;

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
std::vector<CBlock*> CBlockManager::m_blocks = {};	// �u���b�N�̏��
int CBlockManager::m_nNumAll = 0;					// �u���b�N�̑���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlockManager::CBlockManager()
{
	// �l�̃N���A
}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBlockManager::~CBlockManager()
{
	// �Ȃ�
}
//=============================================================================
// ��������
//=============================================================================
CBlock* CBlockManager::CreateBlock(const char* filepath, D3DXVECTOR3 pos)
{
	CBlock* newBlock = CBlock::Create(filepath, pos, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));

	if (newBlock)
	{
		m_blocks.push_back(newBlock);
		m_nNumAll++;			// �����̃J�E���g�A�b�v
	}

	return newBlock;
}
//=============================================================================
// ����������
//=============================================================================
void CBlockManager::Init(void)
{
	// ���I�z�����ɂ��� (�T�C�Y��0�ɂ���)
	m_blocks.clear();
}
//=============================================================================
// �I������
//=============================================================================
void CBlockManager::Uninit(void)
{
	m_nNumAll = 0;

	// ���I�z�����ɂ��� (�T�C�Y��0�ɂ���)
	m_blocks.clear();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBlockManager::Update(void)
{

	//// �v���C���[�̎擾
	//CPlayer* pPlayer = CGame::GetPlayer();

	//// �J�v�Z����OBB�̓����蔻��
	//if (CCollision::CheckCapsuleOBBCollision(pPlayer->GetCollider(), m_pCollider))
	//{

	//}

}
//=============================================================================
// �`�揈��
//=============================================================================
void CBlockManager::Draw(void)
{


}
//=============================================================================
// �u���b�N���̓ǂݍ��ݏ���
//=============================================================================
void CBlockManager::LoadFromJson(const char* filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{// �J���Ȃ�����
		MessageBox(nullptr, "�X�e�[�W�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK | MB_ICONERROR);
		return;
	}

	json j;
	file >> j;

	// �t�@�C�������
	file.close();

	// �����̃u���b�N������
	for (auto block : m_blocks)
	{
		if (block != nullptr)
		{
			// �u���b�N�̏I������
			block->Uninit();
		}
	}

	// ���I�z�����ɂ��� (�T�C�Y��0�ɂ���)
	m_blocks.clear();
	m_nNumAll = 0;// ���������Z�b�g
	
	// �V���ɐ���
	for (const auto& b : j)
	{
		std::string filepath = b["filepath"];
		D3DXVECTOR3 pos(b["pos"][0], b["pos"][1], b["pos"][2]);
		D3DXVECTOR3 degRot(b["rot"][0], b["rot"][1], b["rot"][2]);
		D3DXVECTOR3 size(b["size"][0], b["size"][1], b["size"][2]);

		D3DXVECTOR3 rot = D3DXToRadian(degRot); // �x�����W�A���ɕϊ�

		// �^�C�v����u���b�N����
		CBlock* block = CreateBlock(filepath.c_str(), pos);

		if (!block)
		{
			continue;
		}

		block->SetRot(rot);
		block->SetSize(size);
	}
}
//=============================================================================
// �S�u���b�N�̎擾
//=============================================================================
std::vector<CBlock*>& CBlockManager::GetAllBlocks(void)
{
	return m_blocks;
}