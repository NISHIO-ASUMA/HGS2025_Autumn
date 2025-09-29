//=============================================================================
//
// ブロック処理 [block.h]
// Author : RIKU TANEKAWA
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "objectX.h"
#include "collisionUtils.h"

//*****************************************************************************
// ブロッククラス
//*****************************************************************************
class CBlock : public CObjectX
{
public:
	CBlock(int nPriority = 3);
	virtual ~CBlock() = default;

	static CBlock* Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size);	// ブロックの生成
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// コライダー生成（デフォルトはボックス）
	virtual CBoxCollider* CreateCollider(void)
	{
		return new CBoxCollider(GetModelSize());
	}

	//*****************************************************************************
	// setter関数
	//*****************************************************************************
	void SetType(TYPE type) { m_Type = type; }											// タイプの設定

	//*****************************************************************************
	// getter関数
	//*****************************************************************************
	TYPE GetType(void) const { return m_Type; }											// タイプの取得
	D3DXMATRIX GetWorldMatrix(void);
	CCollider* GetCollider(void) { return m_pCollider; }

private:
	TYPE m_Type;				// 種類
	CBoxCollider* m_pCollider;	// ボックスコライダー
};

// 岩ブロック
class CRockBlock : public CBlock
{
public:

};

#endif