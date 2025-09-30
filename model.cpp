//=============================================================================
//
// モデル処理 [model.cpp]
// Author : RIKU TANEKAWA
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "model.h"
#include "texture.h"
#include "renderer.h"
#include "manager.h"
#include <cstdio>

//=============================================================================
// コンストラクタ
//=============================================================================
CModel::CModel()
{
	// 値のクリア
	memset(m_Path, 0, sizeof(m_Path));	// ファイルパス
	m_nIdxTexture = 0;					// テクスチャインデックス
	m_pos		= VECTOR3_NULL;			// 位置
	m_rot		= VECTOR3_NULL;			// 向き
	m_move		= VECTOR3_NULL;			// 移動量
	m_pMesh		= nullptr;				// メッシュへのポインタ
	m_pBuffMat	= nullptr;				// マテリアルへのポインタ
	m_dwNumMat	= NULL;					// マテリアル数
	m_mtxWorld	= {};					// ワールドマトリックス
	m_pParent	= nullptr;				// 親モデルへのポインタ
}
//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{
	// なし
}
//=============================================================================
// 生成処理
//=============================================================================
CModel* CModel::Create(const char* pFilepath, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CModel* pModel = new CModel;

	pModel->m_pos = pos;
	pModel->m_rot = D3DXToRadian(rot);
	strcpy_s(pModel->m_Path, pFilepath); // パス保存

	// 初期化処理
	pModel->Init();

	return pModel;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{
	// テクスチャの取得
	CTexture* pTexture = CManager::GetTexture();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(m_Path,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;		// 頂点バッファへのポインタ

	// 頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットの取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);


	// 最大値とかはいったんなし

		// AABB計算用の最小・最大値初期化
	D3DXVECTOR3 vMin(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXVECTOR3 vMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 最大値・最小値を求める
	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		D3DXVECTOR3* p = (D3DXVECTOR3*)(pVtxBuff + sizeFVF * nCnt);

		vMin.x = min(vMin.x, p->x);
		vMin.y = min(vMin.y, p->y);
		vMin.z = min(vMin.z, p->z);

		vMax.x = max(vMax.x, p->x);
		vMax.y = max(vMax.y, p->y);
		vMax.z = max(vMax.z, p->z);
	}

	// 頂点バッファのアンロック
	m_pMesh->UnlockVertexBuffer();

	// 元サイズ = 最大 - 最小
	m_modelSize = vMax - vMin;


	D3DXMATERIAL* pMat;// マテリアルへのポインタ

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	m_nIdxTexture = new int[m_dwNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{// テクスチャファイルが存在する
			// テクスチャの登録
			m_nIdxTexture[nCntMat] = pTexture->RegisterDynamic(pMat[nCntMat].pTextureFilename);
		}
		else
		{// テクスチャが存在しない
			m_nIdxTexture[nCntMat] = -1;
		}
	}


	return S_OK;
}
//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	// テクスチャインデックスの破棄
	if (m_nIdxTexture != nullptr)
	{
		delete[] m_nIdxTexture;
		m_nIdxTexture = nullptr;
	}

	// メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	// マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
	// 位置を更新
	m_pos.x += m_move.x;
	m_pos.z += m_move.z;
	m_pos.y += m_move.y;
}
//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	// テクスチャの取得
	CTexture* pTexture = CManager::GetTexture();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用

	D3DXMATERIAL* pMat;		// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + m_OffsetRot.y, m_rot.x + m_OffsetRot.x, m_rot.z + m_OffsetRot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_OffsetPos.x, m_pos.y + m_OffsetPos.y, m_pos.z + m_OffsetPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mtxParent;

	if (m_pParent != nullptr)
	{// 親が存在する
		// 親モデルのマトリックスを取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{// 親が存在しない
		// 現在(最新)のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// 親のマトリックスと掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスを設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_nIdxTexture[nCntMat] == -1)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, nullptr);
		}
		else
		{
			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture[nCntMat]));
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}