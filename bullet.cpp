//==============================
//
// 弾[bullet.cpp]
// Author : Kaiti Aoto
//
//==============================

// インクルード
#include "bullet.h"
#include "renderer.h"
#include "manager.h"
#include "particle.h"
#include "player.h"
#include "debugproc.h"

//==================
// コンストラクタ
//==================
CBullet::CBullet(int nPriority) : CObjectX(nPriority)
{
	// 値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_mtxWorld = {};
	m_pModel = NULL;
	m_type = TYPE_NONE;
	m_user = USER_PLAYER;
	m_nLife = 0;
	m_bUse = true;
	m_bSkill = false;
}
//================
// デストラクタ
//================
CBullet::~CBullet()
{

}
//===========
// 生成処理
//===========
CBullet* CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, USER user)
{
	// 生成
	CBullet* pBullet = new CBullet();
	// 初期化
	if (FAILED(pBullet->Init(pos, rot, user)))
	{// NULLチェック
		delete pBullet;
		return nullptr;
	}
	return pBullet;	// ポインタを返す
}
//===============
// 初期化処理
//===============
HRESULT CBullet::Init(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, USER user)
{
	// 値を代入
	m_pos = pos;
	m_rot = rot;
	m_user = user;

	// メンバ変数を初期化
	m_bUse = true;		// 使用している状態にする
	m_bSkill = false;	// スキルではない状態にする

	// モデル名格納用変数＆その初期化
	const char* pFilename = "data\\MODELS\\Rock_001.x";
	
	// 移動量を設定
	m_move = D3DXVECTOR3(sinf(m_rot.y - D3DX_PI) * BULLET_SPEED, 0.0f, cosf(m_rot.y - D3DX_PI) * BULLET_SPEED);

	// ユーザーがプレイヤーなら
	if (m_user == USER_PLAYER)
	{
		m_nLife = BULLET_LIFE;			// 寿命を設定
	}
	// ユーザーが敵なら
	else if (m_user == USER_ENEMY)
	{
		m_nLife = BULLET_LIFE / 7;	// 寿命を設定
	}

	// モデル生成
	m_pModel = CModel::Create(pFilename, m_pos, m_rot);
	//サイズを設定

	// オブジェクトの種類設定
	SetObjType(TYPE_BULLET);
	
	return S_OK;
}
//============
// 終了処理
//============
void CBullet::Uninit(void)
{	
	if (m_pModel != NULL)
	{// NULLチェック
		// モデルの終了処理＆破棄
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// オブジェクトの破棄
	CObject::Release();
}
//============
// 更新処理
//============
void CBullet::Update(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// ユーザーがプレイヤーなら
		if (m_user == USER_PLAYER)
		{
			// バウンド移動
			Move();
		}
		// ユーザーが敵なら
		else if(m_user == USER_ENEMY)
		{
			// 移動
			Move();
		}
		// 当たり判定
		Collision();

		// 寿命を減らす
		m_nLife--;
		// 寿命が０以下なら
		if (m_nLife <= 0)
		{
			m_bUse = false;	// 使用していない状態に
		}
	}
	// 使っていないなら
	else if (m_bUse == false)
	{
		// 終了処理
		Uninit();
		return;
	}
}
//============
// 描画処理
//============
void CBullet::Draw(void)
{
	// 使用していれば
	if (m_bUse == true)
	{
		// デバイスの取得
		CRenderer* pRenderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

		// 計算用マトリックス
		D3DXMATRIX mtxTrans;
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// ユーザーがプレイヤーなら
		if (m_user == USER_PLAYER)
		{
			// モデル描画
			m_pModel->Draw();
		}
	}
}
//===========
// 移動処理
//===========
void CBullet::Move(void)
{
	// 位置更新
	m_pos += m_move;

	// モデルの位置更新
	m_pModel->SetPos(m_pos);
	m_pModel->SetRot(m_rot);
}
//====================
// 敵との当たり判定
//====================
bool CBullet::Collision(void)
{
	D3DXVECTOR3 contactNormal;	// 接触法線

	// ユーザーがプレイヤーなら
	if (m_user == USER_PLAYER)
	{

	}
	// ユーザーが敵なら
	else if (m_user == USER_ENEMY)
	{

	}


	return false;	 // 判定を返す
}