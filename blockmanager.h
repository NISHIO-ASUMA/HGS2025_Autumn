//=============================================================================
//
// ブロックマネージャー処理 [blockmanager.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "block.h"
#include <vector>


//*****************************************************************************
// ブロックマネージャークラス
//*****************************************************************************
class CBlockManager
{
public:
	CBlockManager();
	~CBlockManager();

    static CBlock* CreateBlock(const char* filepath, D3DXVECTOR3 pos);
    void Init(void);
    void Uninit(void);// 終了処理
    void Update(void);
    void Draw(void);
    void LoadFromJson(const char* filename);

    //*****************************************************************************
    // getter関数
    //*****************************************************************************
    static std::vector<CBlock*>& GetAllBlocks(void);

private:
    static std::vector<CBlock*> m_blocks;   // ブロック情報
    static int m_nNumAll;                   // 総数

};

#endif
