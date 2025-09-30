//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "block.h"
#include "manager.h"
#include "player.h"
#include "characterManager.h"
#include "particle.h"

// 名前空間の使用
using namespace std;

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
std::unordered_map<std::string, std::function<CBlock* ()>> CBlock::blockFactory;

//=============================================================================
// コンストラクタ
//=============================================================================
CBlock::CBlock(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_pCollider = nullptr;
}
//=============================================================================
// 生成処理
//=============================================================================
CBlock* CBlock::Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size)
{
	CBlock* pBlock = nullptr;

	if (blockFactory.empty())
	{
		// ファクトリー
		pBlock->InitFactry();
	}

	auto it = blockFactory.find(pFilepath);
	if (it != blockFactory.end())
	{
		pBlock = it->second(); // ラムダで派生クラスを生成
	}
	else
	{
		pBlock = new CBlock(); // デフォルト
	}

	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->SetSize(size);
	pBlock->SetPath(pFilepath);// モデルパス

	// 初期化処理
	pBlock->Init();

	// ボックスコライダーの生成
	pBlock->m_pCollider = pBlock->CreateCollider();

	return pBlock;
}
//=============================================================================
// ファクトリー処理
//=============================================================================
void CBlock::InitFactry(void)
{
	blockFactory["data/MODELS/convenience_store00.x"] = []() { return new CConveniBlock(); };
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBlock::Init(void)
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CBlock::Uninit(void)
{
	// コライダーの破棄
	if (m_pCollider != nullptr)
	{
		delete m_pCollider;
		m_pCollider = nullptr;
	}

	// オブジェクトXの終了処理
	CObjectX::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBlock::Update(void)
{
	if (m_pCollider)
	{
		// コライダーの更新
		m_pCollider->UpdateTransform(GetPos(), GetRot(), GetSize());
	}

	// プレイヤーの取得
	CPlayer* pPlayer = CCharacterManager::GetInstance().GetCharacter<CPlayer>();

	// カプセルとOBBの当たり判定
	if (CCollision::CheckCapsuleOBBCollision(
		(CCapsuleCollider*)pPlayer->GetCollider(),
		(CBoxCollider*)m_pCollider))
	{
		// カプセルの押し戻し処理
		CCollision::PushCapsuleOutOfOBB((CCapsuleCollider*)pPlayer->GetCollider(),
			(CBoxCollider*)m_pCollider);
	}

	// オブジェクトXの更新処理
	CObjectX::Update();
}
//=============================================================================
// 描画処理
//=============================================================================
void CBlock::Draw(void)
{
	// オブジェクトXの描画処理
	CObjectX::Draw();
}
//=============================================================================
// ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX CBlock::GetWorldMatrix(void)
{
	D3DXMATRIX matScale, matRot, matTrans;

	// スケール行列
	D3DXVECTOR3 scale = GetSize(); // 拡大率
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	// 回転行列
	D3DXVECTOR3 rot = GetRot(); // ラジアン角
	D3DXMatrixRotationYawPitchRoll(&matRot, rot.y, rot.x, rot.z);

	// 平行移動行列
	D3DXVECTOR3 pos = GetPos();
	D3DXMatrixTranslation(&matTrans, pos.x, pos.y, pos.z);

	// 合成：S * R * T
	D3DXMATRIX world = matScale * matRot * matTrans;

	return world;
}


//=============================================================================
// コンビニブロックのコンストラクタ
//=============================================================================
CConveniBlock::CConveniBlock()
{
	// タイプを設定
	SetType(TYPE_CONVENI);
}
//=============================================================================
// コンビニブロックのデストラクタ
//=============================================================================
CConveniBlock::~CConveniBlock()
{
	// なし
}
//=============================================================================
// コンビニブロックの更新処理
//=============================================================================
void CConveniBlock::Update(void)
{
	CBlock::Update();

	//// オフセット
	//D3DXVECTOR3 localOffset(0.0f, 60.0f, 0.0f); // 松明の先端（ローカル）
	//D3DXVECTOR3 worldOffset;

	//// ブロックのワールドマトリックスを取得
	//D3DXMATRIX worldMtx = GetWorldMatrix();

	//D3DXVec3TransformCoord(&worldOffset, &localOffset, &worldMtx);

	//// パーティクル生成
	//CParticle::Create<CFireParticle>(VECTOR3_NULL, worldOffset, D3DXCOLOR(0.8f, 0.5f, 0.1f, 0.8f), 8, 1);
	//CParticle::Create<CFireParticle>(VECTOR3_NULL, worldOffset, D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.8f), 15, 1);
}
