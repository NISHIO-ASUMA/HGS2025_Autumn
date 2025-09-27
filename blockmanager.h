//=============================================================================
//
// �u���b�N�}�l�[�W���[���� [blockmanager.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "block.h"
#include <vector>


//*****************************************************************************
// �u���b�N�}�l�[�W���[�N���X
//*****************************************************************************
class CBlockManager
{
public:
	CBlockManager();
	~CBlockManager();

    static CBlock* CreateBlock(const char* filepath, D3DXVECTOR3 pos);
    void Init(void);
    void Uninit(void);// �I������
    void Update(void);
    void Draw(void);
    void LoadFromJson(const char* filename);

    //*****************************************************************************
    // getter�֐�
    //*****************************************************************************
    static std::vector<CBlock*>& GetAllBlocks(void);

private:
    static std::vector<CBlock*> m_blocks;   // �u���b�N���
    static int m_nNumAll;                   // ����

};

#endif
